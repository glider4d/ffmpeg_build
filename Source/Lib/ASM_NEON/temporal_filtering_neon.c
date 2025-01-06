/*
 * Copyright (c) 2024, Alliance for Open Media. All rights reserved
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at https://www.aomedia.org/license/software-license. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at https://www.aomedia.org/license/patent-license.
*/

#include <assert.h>
#include <arm_neon.h>
#include "definitions.h"
#include "temporal_filtering_constants.h"
#include "utility.h"

/* value [i:0-15] (sqrt((float)i)*65536.0 */
static const uint32_t sqrt_array_fp16[16] = {0,
                                             65536,
                                             92681,
                                             113511,
                                             131072,
                                             146542,
                                             160529,
                                             173391,
                                             185363,
                                             196608,
                                             207243,
                                             217358,
                                             227023,
                                             236293,
                                             245213,
                                             253819};

/* Calc sqrt linear max error 10% */
static uint32_t sqrt_fast(uint32_t x) {
    if (x > 15) {
        const int log2_half = svt_log2f(x) >> 1;
        const int mul2      = log2_half << 1;
        int       base      = x >> (mul2 - 2);
        assert(base < 16);
        return sqrt_array_fp16[base] >> (17 - log2_half);
    }
    return sqrt_array_fp16[x] >> 16;
}

#define SSE_STRIDE (BW + 2)

static uint32_t calculate_squared_errors_sum_no_div_16x16_neon(const uint8_t *s, int s_stride, const uint8_t *p,
                                                               int p_stride) {
    int32x4_t sum_hi = vdupq_n_s32(0);
    int32x4_t sum_lo = vdupq_n_s32(0);

    const int32x2_t stride = vzip1_s32(vdup_n_s32(s_stride), vdup_n_s32(p_stride));

    for (unsigned int i = 0; i < 16; i++) {
        const int32x2_t offset = vmul_s32(stride, vdup_n_s32(i));

        const uint8x16_t s_8 = vld1q_u8(s + vget_lane_s32(offset, 0));
        const uint8x16_t p_8 = vld1q_u8(p + vget_lane_s32(offset, 1));

        const int16x8_t s_8_hi = vreinterpretq_s16_u16(vmovl_u8(vget_high_u8(s_8)));
        const int16x8_t p_8_hi = vreinterpretq_s16_u16(vmovl_u8(vget_high_u8(p_8)));
        const int16x8_t s_8_lo = vreinterpretq_s16_u16(vmovl_u8(vget_low_u8(s_8)));
        const int16x8_t p_8_lo = vreinterpretq_s16_u16(vmovl_u8(vget_low_u8(p_8)));

        const int16x8_t dif_hi = vsubq_s16(s_8_hi, p_8_hi);
        const int16x8_t dif_lo = vsubq_s16(s_8_lo, p_8_lo);

        const int32x4_t pl_hi = vmull_s16(vget_low_s16(dif_hi), vget_low_s16(dif_hi));
        const int32x4_t ph_hi = vmull_high_s16(dif_hi, dif_hi);
        const int32x4_t pl_lo = vmull_s16(vget_low_s16(dif_lo), vget_low_s16(dif_lo));
        const int32x4_t ph_lo = vmull_high_s16(dif_lo, dif_lo);

        sum_hi = vaddq_s32(sum_hi, vpaddq_s32(pl_hi, ph_hi));
        sum_lo = vaddq_s32(sum_lo, vpaddq_s32(pl_lo, ph_lo));
    }

    int32x4_t sum = vpaddq_s32(sum_hi, sum_lo);

    sum = vpaddq_s32(sum, sum);
    sum = vpaddq_s32(sum, sum);

    return vgetq_lane_s32(sum, 0);
}

// T[X] =  exp(-(X)/16)  for x in [0..7], step 1/16 values in Fixed Points shift 16
static const int32_t expf_tab_fp16[] = {
    65536, 61565, 57835, 54331, 51039, 47947, 45042, 42313, 39749, 37341, 35078, 32953, 30957, 29081, 27319,
    25664, 24109, 22648, 21276, 19987, 18776, 17638, 16570, 15566, 14623, 13737, 12904, 12122, 11388, 10698,
    10050, 9441,  8869,  8331,  7827,  7352,  6907,  6488,  6095,  5726,  5379,  5053,  4747,  4459,  4189,
    3935,  3697,  3473,  3262,  3065,  2879,  2704,  2541,  2387,  2242,  2106,  1979,  1859,  1746,  1640,
    1541,  1447,  1360,  1277,  1200,  1127,  1059,  995,   934,   878,   824,   774,   728,   683,   642,
    603,   566,   532,   500,   470,   441,   414,   389,   366,   343,   323,   303,   285,   267,   251,
    236,   222,   208,   195,   184,   172,   162,   152,   143,   134,   126,   118,   111,   104,   98,
    92,    86,    81,    76,    72,    67,    63,    59,    56,    52,    49,    46,    43,    41,    38,
    36,    34,    31,    30,    28,    26,    24,    23,    21};

static void calculate_squared_errors_sum_2x8x8_no_div_neon(const uint8_t *s, int s_stride, const uint8_t *p,
                                                           int p_stride, uint32_t *output) {
    int32x4_t sum_lo = vdupq_n_s32(0);
    int32x4_t sum_hi = vdupq_n_s32(0);

    const int32x2_t stride = vzip1_s32(vdup_n_s32(s_stride), vdup_n_s32(p_stride));

    for (unsigned int i = 0; i < 8; i++) {
        const int32x2_t offset = vmul_s32(stride, vdup_n_s32(i));

        const uint8x16_t s_8 = vld1q_u8(s + vget_lane_s32(offset, 0));
        const uint8x16_t p_8 = vld1q_u8(p + vget_lane_s32(offset, 1));

        const int16x8_t s_8_lo = vreinterpretq_s16_u16(vmovl_u8(vget_low_u8(s_8)));
        const int16x8_t p_8_lo = vreinterpretq_s16_u16(vmovl_u8(vget_low_u8(p_8)));

        const int16x8_t s_8_hi = vreinterpretq_s16_u16(vmovl_u8(vget_high_u8(s_8)));
        const int16x8_t p_8_hi = vreinterpretq_s16_u16(vmovl_u8(vget_high_u8(p_8)));

        const int16x8_t dif_lo = vsubq_s16(s_8_lo, p_8_lo);
        const int16x8_t dif_hi = vsubq_s16(s_8_hi, p_8_hi);

        const int32x4_t pl_lo = vmull_s16(vget_low_s16(dif_lo), vget_low_s16(dif_lo));
        const int32x4_t ph_lo = vmull_high_s16(dif_lo, dif_lo);
        sum_lo                = vaddq_s32(sum_lo, vpaddq_s32(pl_lo, ph_lo));

        const int32x4_t pl_hi = vmull_s16(vget_low_s16(dif_hi), vget_low_s16(dif_hi));
        const int32x4_t ph_hi = vmull_high_s16(dif_hi, dif_hi);
        sum_hi                = vaddq_s32(sum_hi, vpaddq_s32(pl_hi, ph_hi));
    }
    sum_lo = vpaddq_s32(sum_lo, sum_lo);
    sum_lo = vpaddq_s32(sum_lo, sum_lo);

    sum_hi = vpaddq_s32(sum_hi, sum_hi);
    sum_hi = vpaddq_s32(sum_hi, sum_hi);

    output[0] = vgetq_lane_s32(sum_lo, 0);
    output[1] = vgetq_lane_s32(sum_hi, 0);
}

static void svt_av1_apply_temporal_filter_planewise_medium_partial_neon(
    struct MeContext *me_ctx, const uint8_t *y_src, int y_src_stride, const uint8_t *y_pre, int y_pre_stride,
    unsigned int block_width, unsigned int block_height, uint32_t *y_accum, uint16_t *y_count, uint32_t tf_decay_factor,
    uint32_t luma_window_error_quad_fp8[4], int is_chroma) {
    unsigned int i, j, k, subblock_idx;

    int32_t  idx_32x32               = me_ctx->tf_block_col + me_ctx->tf_block_row * 2;
    uint32_t distance_threshold_fp16 = AOMMAX((me_ctx->tf_mv_dist_th << 16) / 10, 1 << 16);
    // Calculation for every quarter
    uint32_t  d_factor_fp8[4];
    uint32_t  block_error_fp8[4];
    uint32_t  chroma_window_error_quad_fp8[4];
    uint32_t *window_error_quad_fp8 = is_chroma ? chroma_window_error_quad_fp8 : luma_window_error_quad_fp8;

    if (me_ctx->tf_32x32_block_split_flag[idx_32x32]) {
        const int32x4_t col = vmovl_s16(vld1_s16((int16_t *)&me_ctx->tf_16x16_mv_x[idx_32x32 * 4]));
        const int32x4_t row = vmovl_s16(vld1_s16((int16_t *)&me_ctx->tf_16x16_mv_y[idx_32x32 * 4]));

        const uint32x4_t hyp     = vreinterpretq_u32_s32(vaddq_s32(vmulq_s32(col, col), vmulq_s32(row, row)));
        const uint32x4_t hyp_256 = vshlq_n_u32(hyp, 8);

        uint32x4_t distance_fp4 = vcombine_u32(vzip1_u32(vdup_n_u32(sqrt_fast(vgetq_lane_u32(hyp_256, 0))),
                                                         vdup_n_u32(sqrt_fast(vgetq_lane_u32(hyp_256, 1)))),
                                               vzip1_u32(vdup_n_u32(sqrt_fast(vgetq_lane_u32(hyp_256, 2))),
                                                         vdup_n_u32(sqrt_fast(vgetq_lane_u32(hyp_256, 3)))));

        uint32x4_t d_factor_fp8_v = vcvtq_u32_f32(
            vdivq_f32(vcvtq_f32_u32(vshlq_n_u32(distance_fp4, 12)),
                      vcvtq_f32_u32(vshrq_n_u32(vdupq_n_u32(distance_threshold_fp16), 8))));
        d_factor_fp8_v = vmaxq_u32(d_factor_fp8_v, vdupq_n_u32(1 << 8));
        vst1q_u32(d_factor_fp8, d_factor_fp8_v);

        // ignore odd elements, since those are the higher 32 bits of every 64 bit entry
        uint32x4x2_t aux = vld2q_u32((uint32_t *)&me_ctx->tf_16x16_block_error[idx_32x32 * 4 + 0]);
        vst1q_u32(block_error_fp8, aux.val[0]);

    } else {
        tf_decay_factor <<= 1;
        int32_t col = me_ctx->tf_32x32_mv_x[idx_32x32];
        int32_t row = me_ctx->tf_32x32_mv_y[idx_32x32];

        uint32_t distance_fp4 = sqrt_fast(((uint32_t)(col * col + row * row)) << 8);
        d_factor_fp8[0] = d_factor_fp8[1] = d_factor_fp8[2] = d_factor_fp8[3] = AOMMAX(
            (distance_fp4 << 12) / (distance_threshold_fp16 >> 8), 1 << 8);
        FP_ASSERT(me_ctx->tf_32x32_block_error[idx_32x32] < ((uint64_t)1 << 30));
        block_error_fp8[0] = block_error_fp8[1] = block_error_fp8[2] = block_error_fp8[3] =
            (uint32_t)(me_ctx->tf_32x32_block_error[idx_32x32] >> 2);
    }

    if (block_width == 32) {
        window_error_quad_fp8[0] = calculate_squared_errors_sum_no_div_16x16_neon(
            y_src, y_src_stride, y_pre, y_pre_stride);
        window_error_quad_fp8[1] = calculate_squared_errors_sum_no_div_16x16_neon(
            y_src + 16, y_src_stride, y_pre + 16, y_pre_stride);
        window_error_quad_fp8[2] = calculate_squared_errors_sum_no_div_16x16_neon(
            y_src + y_src_stride * 16, y_src_stride, y_pre + y_pre_stride * 16, y_pre_stride);
        window_error_quad_fp8[3] = calculate_squared_errors_sum_no_div_16x16_neon(
            y_src + y_src_stride * 16 + 16, y_src_stride, y_pre + y_pre_stride * 16 + 16, y_pre_stride);
    } else {
        calculate_squared_errors_sum_2x8x8_no_div_neon(y_src, y_src_stride, y_pre, y_pre_stride, window_error_quad_fp8);

        calculate_squared_errors_sum_2x8x8_no_div_neon(
            y_src + y_src_stride * 8, y_src_stride, y_pre + y_pre_stride * 8, y_pre_stride, &window_error_quad_fp8[2]);

        uint32x4_t window_error_quad_fp8_v = vld1q_u32(window_error_quad_fp8);
        window_error_quad_fp8_v            = vshlq_n_u32(window_error_quad_fp8_v, 2);

        vst1q_u32(window_error_quad_fp8, window_error_quad_fp8_v);
    }

    if (is_chroma) {
        for (i = 0; i < 4; ++i) {
            FP_ASSERT(((int64_t)window_error_quad_fp8[i] * 5 + luma_window_error_quad_fp8[i]) < ((int64_t)1 << 31));
        }

        uint32x4_t window_error_quad_fp8_v      = vld1q_u32(window_error_quad_fp8);
        uint32x4_t luma_window_error_quad_fp8_v = vld1q_u32(luma_window_error_quad_fp8);

        window_error_quad_fp8_v = vmlaq_u32(luma_window_error_quad_fp8_v, window_error_quad_fp8_v, vdupq_n_u32(5));
        window_error_quad_fp8_v = vcvtq_u32_f32(vdivq_f32(vcvtq_f32_u32(window_error_quad_fp8_v), vdupq_n_f32(6.0f)));

        vst1q_u32(window_error_quad_fp8, window_error_quad_fp8_v);
    }

    int16x8_t adjusted_weight_int16[4];
    int32x4_t adjusted_weight_int32[4];

    for (subblock_idx = 0; subblock_idx < 4; subblock_idx++) {
        uint32_t combined_error_fp8 = (window_error_quad_fp8[subblock_idx] * TF_WINDOW_BLOCK_BALANCE_WEIGHT +
                                       block_error_fp8[subblock_idx]) /
            (TF_WINDOW_BLOCK_BALANCE_WEIGHT + 1);

        uint64_t avg_err_fp10    = ((combined_error_fp8 >> 3) * (d_factor_fp8[subblock_idx] >> 3));
        uint32_t scaled_diff16   = (uint32_t)AOMMIN((avg_err_fp10) / AOMMAX((tf_decay_factor >> 10), 1), 7 * 16);
        uint32_t adjusted_weight = (expf_tab_fp16[scaled_diff16] * TF_WEIGHT_SCALE) >> 16;

        adjusted_weight_int16[subblock_idx] = vdupq_n_s16((int16_t)(adjusted_weight));
        adjusted_weight_int32[subblock_idx] = vdupq_n_s32((int32_t)(adjusted_weight));
    }

    for (i = 0; i < block_height; i++) {
        const int subblock_idx_h = (i >= block_height / 2) * 2;
        for (j = 0; j < block_width; j += 8) {
            k = i * y_pre_stride + j;

            uint16x8_t count_array = vld1q_u16(y_count + k);

            count_array = vaddq_u16(
                count_array, vreinterpretq_u16_s16(adjusted_weight_int16[subblock_idx_h + (j >= block_width / 2)]));

            vst1q_u16(y_count + k, count_array);
            uint32x4_t accumulator_array1 = vld1q_u32(y_accum + k);
            uint32x4_t accumulator_array2 = vld1q_u32(y_accum + k + 4);

            uint16x8_t frame2_array       = vmovl_u8(vld1_u8(y_pre + k));
            uint32x4_t frame2_array_u32_1 = vmovl_u16(vget_low_u16(frame2_array));
            uint32x4_t frame2_array_u32_2 = vmovl_u16(vget_high_u16(frame2_array));

            uint32x4_t adj_weight = vreinterpretq_u32_s32(
                adjusted_weight_int32[subblock_idx_h + (j >= block_width / 2)]);

            frame2_array_u32_1 = vmulq_u32(frame2_array_u32_1, adj_weight);
            frame2_array_u32_2 = vmulq_u32(frame2_array_u32_2, adj_weight);

            accumulator_array1 = vaddq_u32(accumulator_array1, frame2_array_u32_1);
            accumulator_array2 = vaddq_u32(accumulator_array2, frame2_array_u32_2);

            vst1q_u32(y_accum + k, accumulator_array1);
            vst1q_u32(y_accum + k + 4, accumulator_array2);
        }
    }
}

void svt_av1_apply_temporal_filter_planewise_medium_neon(
    struct MeContext *me_ctx, const uint8_t *y_src, int y_src_stride, const uint8_t *y_pre, int y_pre_stride,
    const uint8_t *u_src, const uint8_t *v_src, int uv_src_stride, const uint8_t *u_pre, const uint8_t *v_pre,
    int uv_pre_stride, unsigned int block_width, unsigned int block_height, int ss_x, int ss_y, uint32_t *y_accum,
    uint16_t *y_count, uint32_t *u_accum, uint16_t *u_count, uint32_t *v_accum, uint16_t *v_count) {
    uint32_t luma_window_error_quad_fp8[4];

    svt_av1_apply_temporal_filter_planewise_medium_partial_neon(me_ctx,
                                                                y_src,
                                                                y_src_stride,
                                                                y_pre,
                                                                y_pre_stride,
                                                                (unsigned int)block_width,
                                                                (unsigned int)block_height,
                                                                y_accum,
                                                                y_count,
                                                                me_ctx->tf_decay_factor_fp16[C_Y],
                                                                luma_window_error_quad_fp8,
                                                                0);

    if (me_ctx->tf_chroma) {
        svt_av1_apply_temporal_filter_planewise_medium_partial_neon(me_ctx,
                                                                    u_src,
                                                                    uv_src_stride,
                                                                    u_pre,
                                                                    uv_pre_stride,
                                                                    (unsigned int)block_width >> ss_x,
                                                                    (unsigned int)block_height >> ss_y,
                                                                    u_accum,
                                                                    u_count,
                                                                    me_ctx->tf_decay_factor_fp16[C_U],
                                                                    luma_window_error_quad_fp8,
                                                                    1);

        svt_av1_apply_temporal_filter_planewise_medium_partial_neon(me_ctx,
                                                                    v_src,
                                                                    uv_src_stride,
                                                                    v_pre,
                                                                    uv_pre_stride,
                                                                    (unsigned int)block_width >> ss_x,
                                                                    (unsigned int)block_height >> ss_y,
                                                                    v_accum,
                                                                    v_count,
                                                                    me_ctx->tf_decay_factor_fp16[C_V],
                                                                    luma_window_error_quad_fp8,
                                                                    1);
    }
}

// Divide two int32x4 vectors
static uint32x4_t div_u32(const uint32x4_t *a, const uint32x4_t *b) {
    uint32x4_t result = vdupq_n_u32(0);
    result            = vsetq_lane_u32(vdups_laneq_u32(*a, 0) / vdups_laneq_u32(*b, 0), result, 0);
    result            = vsetq_lane_u32(vdups_laneq_u32(*a, 1) / vdups_laneq_u32(*b, 1), result, 1);
    result            = vsetq_lane_u32(vdups_laneq_u32(*a, 2) / vdups_laneq_u32(*b, 2), result, 2);
    result            = vsetq_lane_u32(vdups_laneq_u32(*a, 3) / vdups_laneq_u32(*b, 3), result, 3);
    return result;
}

static void process_block_hbd_neon(int h, int w, uint16_t *buff_hbd_start, uint32_t *accum, uint16_t *count,
                                   uint32_t stride) {
    int i, j;
    int pos = 0;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j += 8) {
            //buff_lbd_start[pos] = (uint8_t)OD_DIVU(accum[k] + (count[k] >> 1), count[k]);
            //buff_lbd_start[pos] = (uint8_t)((accum[k] + (count[k] >> 1))/ count[k]);
            uint32x4_t accum_a = vld1q_u32(accum);
            uint32x4_t accum_b = vld1q_u32(&accum[4]);
            accum += 8;

            uint32x4_t count_a = vmovl_u16(vld1_u16(count));
            uint32x4_t count_b = vmovl_u16(vld1_u16(&count[4]));
            count += 8;

            //accum[k] + (count[k] >> 1)
            uint32x4_t tmp_a = vaddq_u32(accum_a, vshrq_n_u32(count_a, 1));
            uint32x4_t tmp_b = vaddq_u32(accum_b, vshrq_n_u32(count_b, 1));

            //accum[k] + (count[k] >> 1))/ count[k]
            tmp_a             = div_u32(&tmp_a, &count_a);
            tmp_b             = div_u32(&tmp_b, &count_b);
            uint16x8_t tmp_ab = vqmovn_high_u32(vqmovn_u32(tmp_a), tmp_b);

            vst1q_u16(buff_hbd_start + pos, tmp_ab);

            pos += 8;
        }
        pos += stride;
    }
}

static void process_block_lbd_neon(int h, int w, uint8_t *buff_lbd_start, uint32_t *accum, uint16_t *count,
                                   uint32_t stride) {
    int i, j;
    int pos = 0;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j += 8) {
            //buff_lbd_start[pos] = (uint8_t)OD_DIVU(accum[k] + (count[k] >> 1), count[k]);
            //buff_lbd_start[pos] = (uint8_t)((accum[k] + (count[k] >> 1))/ count[k]);
            uint32x4_t accum_a = vld1q_u32(accum);
            uint32x4_t accum_b = vld1q_u32(&accum[4]);
            accum += 8;

            uint32x4_t count_a = vmovl_u16(vld1_u16(count));
            uint32x4_t count_b = vmovl_u16(vld1_u16(&count[4]));
            count += 8;

            //accum[k] + (count[k] >> 1)
            uint32x4_t tmp_a = vaddq_u32(accum_a, vshrq_n_u32(count_a, 1));
            uint32x4_t tmp_b = vaddq_u32(accum_b, vshrq_n_u32(count_b, 1));

            //accum[k] + (count[k] >> 1))/ count[k]
            tmp_a              = div_u32(&tmp_a, &count_a);
            tmp_b              = div_u32(&tmp_b, &count_b);
            uint16x8_t tmp_ab1 = vqmovn_high_u32(vqmovn_u32(tmp_a), tmp_b);

            vst1_u8(buff_lbd_start + pos, vqmovn_u16(tmp_ab1));

            pos += 8;
        }
        pos += stride;
    }
}

void svt_aom_get_final_filtered_pixels_neon(MeContext *me_ctx, EbByte *src_center_ptr_start,
                                            uint16_t **altref_buffer_highbd_start, uint32_t **accum, uint16_t **count,
                                            const uint32_t *stride, int blk_y_src_offset, int blk_ch_src_offset,
                                            uint16_t blk_width_ch, uint16_t blk_height_ch, Bool is_highbd) {
    assert(blk_width_ch % 16 == 0);
    assert(BW % 16 == 0);

    if (!is_highbd) {
        //Process luma
        process_block_lbd_neon(
            BH, BW, &src_center_ptr_start[C_Y][blk_y_src_offset], accum[C_Y], count[C_Y], stride[C_Y] - BW);
        // Process chroma
        if (me_ctx->tf_chroma) {
            process_block_lbd_neon(blk_height_ch,
                                   blk_width_ch,
                                   &src_center_ptr_start[C_U][blk_ch_src_offset],
                                   accum[C_U],
                                   count[C_U],
                                   stride[C_U] - blk_width_ch);
            process_block_lbd_neon(blk_height_ch,
                                   blk_width_ch,
                                   &src_center_ptr_start[C_V][blk_ch_src_offset],
                                   accum[C_V],
                                   count[C_V],
                                   stride[C_V] - blk_width_ch);
        }
    } else {
        // Process luma
        process_block_hbd_neon(
            BH, BW, &altref_buffer_highbd_start[C_Y][blk_y_src_offset], accum[C_Y], count[C_Y], stride[C_Y] - BW);
        // Process chroma
        if (me_ctx->tf_chroma) {
            process_block_hbd_neon(blk_height_ch,
                                   blk_width_ch,
                                   &altref_buffer_highbd_start[C_U][blk_ch_src_offset],
                                   accum[C_U],
                                   count[C_U],
                                   stride[C_U] - blk_width_ch);
            process_block_hbd_neon(blk_height_ch,
                                   blk_width_ch,
                                   &altref_buffer_highbd_start[C_V][blk_ch_src_offset],
                                   accum[C_V],
                                   count[C_V],
                                   stride[C_V] - blk_width_ch);
        }
    }
}

int32_t svt_estimate_noise_fp16_neon(const uint8_t *src, uint16_t width, uint16_t height, uint16_t stride_y) {
    int64_t sum = 0;
    int64_t num = 0;

    //  A | B | C
    //  D | E | F
    //  G | H | I
    // g_x = (A - I) + (G - C) + 2*(D - F)
    // g_y = (A - I) - (G - C) + 2*(B - H)
    // v   = 4*E - 2*(D+F+B+H) + (A+C+G+I)

    const int16x8_t zero               = vdupq_n_s16(0);
    const int16x8_t edge_treshold      = vdupq_n_s16(EDGE_THRESHOLD);
    int32x4_t       num_accumulator_lo = vdupq_n_s32(0);
    int32x4_t       num_accumulator_hi = vdupq_n_s32(0);
    int32x4_t       sum_accumulator_lo = vdupq_n_s32(0);
    int32x4_t       sum_accumulator_hi = vdupq_n_s32(0);

    for (int i = 1; i < height - 1; ++i) {
        const int k_stride = i * stride_y;

        int j = 1;

        int16x8_t rowAC = vreinterpretq_s16_u16(vmovl_u8(vld1_u8(&src[k_stride + j - stride_y - 1 + 0])));
        int16x8_t rowBC = vreinterpretq_s16_u16(vmovl_u8(vld1_u8(&src[k_stride + j - 1 + 0])));
        int16x8_t rowCC = vreinterpretq_s16_u16(vmovl_u8(vld1_u8(&src[k_stride + j + stride_y - 1 + 0])));

        for (; j + 16 < width - 1; j += 16) {
            const uint8x16_t rowABC = vld1q_u8(&src[k_stride + j - stride_y - 1 + 8]);
            const uint8x16_t rowBBC = vld1q_u8(&src[k_stride + j - 1 + 8]);
            const uint8x16_t rowCBC = vld1q_u8(&src[k_stride + j + stride_y - 1 + 8]);

            const int16x8_t rowAA = rowAC;
            const int16x8_t rowAB = vreinterpretq_s16_u16(vmovl_u8(vget_low_u8(rowABC)));
            rowAC                 = vreinterpretq_s16_u16(vmovl_u8(vget_high_u8(rowABC)));
            const int16x8_t rowBA = rowBC;
            const int16x8_t rowBB = vreinterpretq_s16_u16(vmovl_u8(vget_low_u8(rowBBC)));
            rowBC                 = vreinterpretq_s16_u16(vmovl_u8(vget_high_u8(rowBBC)));
            const int16x8_t rowCA = rowCC;
            const int16x8_t rowCB = vreinterpretq_s16_u16(vmovl_u8(vget_low_u8(rowCBC)));
            rowCC                 = vreinterpretq_s16_u16(vmovl_u8(vget_high_u8(rowCBC)));

            const int16x8_t A_lo = rowAA;
            const int16x8_t A_hi = rowAB;
            const int16x8_t B_lo = vextq_s16(rowAA, rowAB, 1);
            const int16x8_t B_hi = vextq_s16(rowAB, rowAC, 1);
            const int16x8_t C_lo = vextq_s16(rowAA, rowAB, 2);
            const int16x8_t C_hi = vextq_s16(rowAB, rowAC, 2);

            const int16x8_t D_lo = rowBA;
            const int16x8_t D_hi = rowBB;
            const int16x8_t E_lo = vextq_s16(rowBA, rowBB, 1);
            const int16x8_t E_hi = vextq_s16(rowBB, rowBC, 1);
            const int16x8_t F_lo = vextq_s16(rowBA, rowBB, 2);
            const int16x8_t F_hi = vextq_s16(rowBB, rowBC, 2);

            const int16x8_t G_lo = rowCA;
            const int16x8_t G_hi = rowCB;
            const int16x8_t H_lo = vextq_s16(rowCA, rowCB, 1);
            const int16x8_t H_hi = vextq_s16(rowCB, rowCC, 1);
            const int16x8_t I_lo = vextq_s16(rowCA, rowCB, 2);
            const int16x8_t I_hi = vextq_s16(rowCB, rowCC, 2);

            const int16x8_t A_m_I_lo = vsubq_s16(A_lo, I_lo);
            const int16x8_t A_m_I_hi = vsubq_s16(A_hi, I_hi);
            const int16x8_t G_m_C_lo = vsubq_s16(G_lo, C_lo);
            const int16x8_t G_m_C_hi = vsubq_s16(G_hi, C_hi);

            const int16x8_t D_m_Fx2_lo = vshlq_n_s16(vsubq_s16(D_lo, F_lo), 1);
            const int16x8_t D_m_Fx2_hi = vshlq_n_s16(vsubq_s16(D_hi, F_hi), 1);
            const int16x8_t B_m_Hx2_lo = vshlq_n_s16(vsubq_s16(B_lo, H_lo), 1);
            const int16x8_t B_m_Hx2_hi = vshlq_n_s16(vsubq_s16(B_hi, H_hi), 1);

            const int16x8_t gx_256_lo = vqabsq_s16(vaddq_s16(vaddq_s16(A_m_I_lo, G_m_C_lo), D_m_Fx2_lo));
            const int16x8_t gx_256_hi = vqabsq_s16(vaddq_s16(vaddq_s16(A_m_I_hi, G_m_C_hi), D_m_Fx2_hi));

            const int16x8_t gy_256_lo = vqabsq_s16(vaddq_s16(vsubq_s16(A_m_I_lo, G_m_C_lo), B_m_Hx2_lo));
            const int16x8_t gy_256_hi = vqabsq_s16(vaddq_s16(vsubq_s16(A_m_I_hi, G_m_C_hi), B_m_Hx2_hi));

            const int16x8_t ga_256_lo = vaddq_s16(gx_256_lo, gy_256_lo);
            const int16x8_t ga_256_hi = vaddq_s16(gx_256_hi, gy_256_hi);

            const int16x8_t D_F_B_Hx2_lo = vshlq_n_s16(vaddq_s16(vaddq_s16(D_lo, F_lo), vaddq_s16(B_lo, H_lo)), 1);
            const int16x8_t D_F_B_Hx2_hi = vshlq_n_s16(vaddq_s16(vaddq_s16(D_hi, F_hi), vaddq_s16(B_hi, H_hi)), 1);
            const int16x8_t A_C_G_I_lo   = vaddq_s16(vaddq_s16(A_lo, C_lo), vaddq_s16(G_lo, I_lo));
            const int16x8_t A_C_G_I_hi   = vaddq_s16(vaddq_s16(A_hi, C_hi), vaddq_s16(G_hi, I_hi));
            int16x8_t       v_256_lo = vqabsq_s16(vaddq_s16(vsubq_s16(vshlq_n_s16(E_lo, 2), D_F_B_Hx2_lo), A_C_G_I_lo));
            int16x8_t       v_256_hi = vqabsq_s16(vaddq_s16(vsubq_s16(vshlq_n_s16(E_hi, 2), D_F_B_Hx2_hi), A_C_G_I_hi));

            //if (ga < EDGE_THRESHOLD)
            const int16x8_t cmp_lo = vreinterpretq_s16_u16(vshrq_n_u16(vcgtq_s16(edge_treshold, ga_256_lo), 15));
            const int16x8_t cmp_hi = vreinterpretq_s16_u16(vshrq_n_u16(vcgtq_s16(edge_treshold, ga_256_hi), 15));
            v_256_lo               = vmulq_s16(v_256_lo, cmp_lo);
            v_256_hi               = vmulq_s16(v_256_hi, cmp_hi);

            //num_accumulator and sum_accumulator have 32bit values
            num_accumulator_lo = vaddq_s32(num_accumulator_lo, vreinterpretq_s32_s16(vzip1q_s16(cmp_lo, zero)));
            num_accumulator_hi = vaddq_s32(num_accumulator_hi, vreinterpretq_s32_s16(vzip1q_s16(cmp_hi, zero)));
            num_accumulator_lo = vaddq_s32(num_accumulator_lo, vreinterpretq_s32_s16(vzip2q_s16(cmp_lo, zero)));
            num_accumulator_hi = vaddq_s32(num_accumulator_hi, vreinterpretq_s32_s16(vzip2q_s16(cmp_hi, zero)));

            sum_accumulator_lo = vaddq_s32(sum_accumulator_lo, vreinterpretq_s32_s16(vzip1q_s16(v_256_lo, zero)));
            sum_accumulator_hi = vaddq_s32(sum_accumulator_hi, vreinterpretq_s32_s16(vzip1q_s16(v_256_hi, zero)));
            sum_accumulator_lo = vaddq_s32(sum_accumulator_lo, vreinterpretq_s32_s16(vzip2q_s16(v_256_lo, zero)));
            sum_accumulator_hi = vaddq_s32(sum_accumulator_hi, vreinterpretq_s32_s16(vzip2q_s16(v_256_hi, zero)));
        }

        for (; j < width - 1; ++j) {
            const int k = i * stride_y + j;

            // Sobel gradients
            const int g_x = (src[k - stride_y - 1] - src[k - stride_y + 1]) +
                (src[k + stride_y - 1] - src[k + stride_y + 1]) + 2 * (src[k - 1] - src[k + 1]);
            const int g_y = (src[k - stride_y - 1] - src[k + stride_y - 1]) +
                (src[k - stride_y + 1] - src[k + stride_y + 1]) + 2 * (src[k - stride_y] - src[k + stride_y]);
            const int ga = abs(g_x) + abs(g_y);

            if (ga < EDGE_THRESHOLD) { // Do not consider edge pixels to estimate the noise
                // Find Laplacian
                const int v = 4 * src[k] - 2 * (src[k - 1] + src[k + 1] + src[k - stride_y] + src[k + stride_y]) +
                    (src[k - stride_y - 1] + src[k - stride_y + 1] + src[k + stride_y - 1] + src[k + stride_y + 1]);
                sum += abs(v);
                ++num;
            }
        }
    }

    int32x4_t sum_256_lo = vpaddq_s32(sum_accumulator_lo, num_accumulator_lo);
    int32x4_t sum_256_hi = vpaddq_s32(sum_accumulator_hi, num_accumulator_hi);
    sum_256_lo           = vpaddq_s32(sum_256_lo, sum_256_lo);
    sum_256_hi           = vpaddq_s32(sum_256_hi, sum_256_hi);

    int32x4_t sum_128 = vaddq_s32(sum_256_lo, sum_256_hi);

    sum += vgetq_lane_s32(sum_128, 0);
    num += vgetq_lane_s32(vextq_s32(sum_128, vdupq_n_s32(0), 1), 0);

    // If very few smooth pels, return -1 since the estimate is unreliable
    if (num < SMOOTH_THRESHOLD) {
        return -65536 /*-1:fp16*/;
    }

    FP_ASSERT((((int64_t)sum * SQRT_PI_BY_2_FP16) / (6 * num)) < ((int64_t)1 << 31));
    return (int32_t)((sum * SQRT_PI_BY_2_FP16) / (6 * num));
}

static void apply_filtering_central_loop_lbd(uint16_t w, uint16_t h, uint8_t *src, uint16_t src_stride, uint32_t *accum,
                                             uint16_t *count) {
    assert(w % 8 == 0);

    uint32x4_t modifier       = vdupq_n_u32(TF_PLANEWISE_FILTER_WEIGHT_SCALE);
    uint16x8_t modifier_epi16 = vdupq_n_u16(TF_PLANEWISE_FILTER_WEIGHT_SCALE);

    for (uint16_t k = 0, i = 0; i < h; i++) {
        for (uint16_t j = 0; j < w; j += 8) {
            const uint16x8_t src_16 = vmovl_u8(vld1_u8(src + i * src_stride + j));

            vst1q_u32(accum + k + 0, vmulq_u32(modifier, vmovl_u16(vget_low_u16(src_16))));
            vst1q_u32(accum + k + 4, vmulq_u32(modifier, vmovl_u16(vget_high_u16(src_16))));
            vst1q_u16(count + k, modifier_epi16);

            k += 8;
        }
    }
}

static uint32_t calculate_squared_errors_sum_no_div_highbd_neon(const uint16_t *s, int s_stride, const uint16_t *p,
                                                                int p_stride, unsigned int w, unsigned int h,
                                                                int const shift_factor) {
    assert(w % 16 == 0 && "block width must be multiple of 16");
    unsigned int i, j;

    int32x4_t sum = vdupq_n_s32(0);

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j += 8) {
            const uint16x8_t s_16 = vld1q_u16(s + i * s_stride + j);
            const uint16x8_t p_16 = vld1q_u16(p + i * p_stride + j);

            const int16x8_t dif = vreinterpretq_s16_u16(vsubq_u16(s_16, p_16));
            sum = vaddq_s32(sum, vpaddq_s32(vmull_s16(vget_low_s16(dif), vget_low_s16(dif)), vmull_high_s16(dif, dif)));
        }
    }

    sum = vpaddq_s32(sum, sum);
    sum = vpaddq_s32(sum, sum);

    return vgetq_lane_s32(vrshlq_s32(sum, vdupq_n_s32(-shift_factor)), 0);
}

static void calculate_squared_errors_sum_2x8xh_no_div_highbd_neon(const uint16_t *s, int s_stride, const uint16_t *p,
                                                                  int p_stride, unsigned int h, int shift_factor,
                                                                  uint32_t *output) {
    const int32x4_t zero      = vdupq_n_s32(0);
    int32x4_t       sum_0     = zero;
    int32x4_t       sum_1     = zero;
    const int32x4_t shift_vec = vdupq_n_s32(-shift_factor);

    for (unsigned int i = 0; i < h; i++) {
        const uint16x8_t s_8_0 = vld1q_u16(s + i * s_stride);
        const uint16x8_t s_8_1 = vld1q_u16(s + i * s_stride + 8);
        const uint16x8_t p_8_0 = vld1q_u16(p + i * p_stride);
        const uint16x8_t p_8_1 = vld1q_u16(p + i * p_stride + 8);

        const int16x8_t dif_0 = vreinterpretq_s16_u16(vsubq_u16(s_8_0, p_8_0));
        const int16x8_t dif_1 = vreinterpretq_s16_u16(vsubq_u16(s_8_1, p_8_1));

        sum_0 = vaddq_s32(
            sum_0, vpaddq_s32(vmull_s16(vget_low_s16(dif_0), vget_low_s16(dif_0)), vmull_high_s16(dif_0, dif_0)));
        sum_1 = vaddq_s32(
            sum_1, vpaddq_s32(vmull_s16(vget_low_s16(dif_1), vget_low_s16(dif_1)), vmull_high_s16(dif_1, dif_1)));
    }
    sum_0 = vpaddq_s32(sum_0, sum_0);
    sum_0 = vpaddq_s32(sum_0, sum_0);
    sum_1 = vpaddq_s32(sum_1, sum_1);
    sum_1 = vpaddq_s32(sum_1, sum_1);

    output[0] = vgetq_lane_s32(vrshlq_s32(sum_0, shift_vec), 0);
    output[1] = vgetq_lane_s32(vrshlq_s32(sum_1, shift_vec), 0);
}

static void svt_av1_apply_temporal_filter_planewise_medium_hbd_partial_neon(
    struct MeContext *me_ctx, const uint16_t *y_src, int y_src_stride, const uint16_t *y_pre, int y_pre_stride,
    unsigned int block_width, unsigned int block_height, uint32_t *y_accum, uint16_t *y_count, uint32_t tf_decay_factor,
    uint32_t luma_window_error_quad_fp8[4], int is_chroma, uint32_t encoder_bit_depth) {
    unsigned int i, j, k, subblock_idx;

    const int32_t  idx_32x32               = me_ctx->tf_block_col + me_ctx->tf_block_row * 2;
    const int      shift_factor            = ((encoder_bit_depth - 8) * 2);
    const uint32_t distance_threshold_fp16 = AOMMAX((me_ctx->tf_mv_dist_th << 16) / 10,
                                                    1 << 16); //TODO Change to FP8

    //Calculation for every quarter
    uint32_t  d_factor_fp8[4];
    uint32_t  block_error_fp8[4];
    uint32_t  chroma_window_error_quad_fp8[4];
    uint32_t *window_error_quad_fp8 = is_chroma ? chroma_window_error_quad_fp8 : luma_window_error_quad_fp8;

    if (me_ctx->tf_32x32_block_split_flag[idx_32x32]) {
        for (i = 0; i < 4; ++i) {
            const int32_t  col          = me_ctx->tf_16x16_mv_x[idx_32x32 * 4 + i];
            const int32_t  row          = me_ctx->tf_16x16_mv_y[idx_32x32 * 4 + i];
            const uint32_t distance_fp4 = sqrt_fast(((uint32_t)(col * col + row * row)) << 8);
            d_factor_fp8[i]             = AOMMAX((distance_fp4 << 12) / (distance_threshold_fp16 >> 8), 1 << 8);
            FP_ASSERT(me_ctx->tf_16x16_block_error[idx_32x32 * 4 + i] < ((uint64_t)1 << 35));
            block_error_fp8[i] = (uint32_t)(me_ctx->tf_16x16_block_error[idx_32x32 * 4 + i] >> 4);
        }
    } else {
        tf_decay_factor <<= 1;
        const int32_t col = me_ctx->tf_32x32_mv_x[idx_32x32];
        const int32_t row = me_ctx->tf_32x32_mv_y[idx_32x32];

        const uint32_t distance_fp4 = sqrt_fast(((uint32_t)(col * col + row * row)) << 8);
        d_factor_fp8[0] = d_factor_fp8[1] = d_factor_fp8[2] = d_factor_fp8[3] = AOMMAX(
            (distance_fp4 << 12) / (distance_threshold_fp16 >> 8), 1 << 8);
        FP_ASSERT(me_ctx->tf_32x32_block_error[idx_32x32] < ((uint64_t)1 << 35));
        block_error_fp8[0] = block_error_fp8[1] = block_error_fp8[2] = block_error_fp8[3] =
            (uint32_t)(me_ctx->tf_32x32_block_error[idx_32x32] >> 6);
    }

    if (block_width == 32) {
        window_error_quad_fp8[0] = calculate_squared_errors_sum_no_div_highbd_neon(
            y_src, y_src_stride, y_pre, y_pre_stride, 16, 16, shift_factor);
        window_error_quad_fp8[1] = calculate_squared_errors_sum_no_div_highbd_neon(
            y_src + 16, y_src_stride, y_pre + 16, y_pre_stride, 16, 16, shift_factor);
        window_error_quad_fp8[2] = calculate_squared_errors_sum_no_div_highbd_neon(
            y_src + y_src_stride * 16, y_src_stride, y_pre + y_pre_stride * 16, y_pre_stride, 16, 16, shift_factor);
        window_error_quad_fp8[3] = calculate_squared_errors_sum_no_div_highbd_neon(y_src + y_src_stride * 16 + 16,
                                                                                   y_src_stride,
                                                                                   y_pre + y_pre_stride * 16 + 16,
                                                                                   y_pre_stride,
                                                                                   16,
                                                                                   16,
                                                                                   shift_factor);

    } else {
        calculate_squared_errors_sum_2x8xh_no_div_highbd_neon(
            y_src, y_src_stride, y_pre, y_pre_stride, 8, shift_factor, window_error_quad_fp8);

        calculate_squared_errors_sum_2x8xh_no_div_highbd_neon(y_src + y_src_stride * 8,
                                                              y_src_stride,
                                                              y_pre + y_pre_stride * 8,
                                                              y_pre_stride,
                                                              8,
                                                              shift_factor,
                                                              &window_error_quad_fp8[2]);
        window_error_quad_fp8[0] <<= 2;
        window_error_quad_fp8[1] <<= 2;
        window_error_quad_fp8[2] <<= 2;
        window_error_quad_fp8[3] <<= 2;
    }

    if (is_chroma) {
        for (i = 0; i < 4; ++i) {
            FP_ASSERT(((int64_t)window_error_quad_fp8[i] * 5 + luma_window_error_quad_fp8[i]) < ((int64_t)1 << 31));
            window_error_quad_fp8[i] = (window_error_quad_fp8[i] * 5 + luma_window_error_quad_fp8[i]) / 6;
        }
    }

    uint16x8_t adjusted_weight_int16[4];
    uint32x4_t adjusted_weight_int32[4];

    for (subblock_idx = 0; subblock_idx < 4; subblock_idx++) {
        const uint32_t combined_error_fp8 = (window_error_quad_fp8[subblock_idx] * TF_WINDOW_BLOCK_BALANCE_WEIGHT +
                                             block_error_fp8[subblock_idx]) /
            (TF_WINDOW_BLOCK_BALANCE_WEIGHT + 1);

        const uint64_t avg_err_fp10  = ((combined_error_fp8 >> 3) * (d_factor_fp8[subblock_idx] >> 3));
        uint32_t       scaled_diff16 = (uint32_t)AOMMIN(
            /*((16*avg_err)<<8)*/ (avg_err_fp10) / AOMMAX((tf_decay_factor >> 10), 1), 7 * 16);
        const int adjusted_weight = (expf_tab_fp16[scaled_diff16] * TF_WEIGHT_SCALE) >> 16;

        adjusted_weight_int16[subblock_idx] = vdupq_n_u16(adjusted_weight);
        adjusted_weight_int32[subblock_idx] = vdupq_n_u32(adjusted_weight);
    }

    for (i = 0; i < block_height; i++) {
        const int subblock_idx_h = (i >= block_height / 2) * 2;
        for (j = 0; j < block_width; j += 8) {
            k = i * y_pre_stride + j;

            //y_count[k] += adjusted_weight;
            uint16x8_t count_array = vld1q_u16(y_count + k);
            count_array = vaddq_u16(count_array, adjusted_weight_int16[subblock_idx_h + (j >= block_width / 2)]);
            vst1q_u16(y_count + k, count_array);

            //y_accum[k] += adjusted_weight * pixel_value;
            uint32x4_t       accumulator_array1 = vld1q_u32(y_accum + k);
            uint32x4_t       accumulator_array2 = vld1q_u32(y_accum + k + 4);
            const uint16x8_t frame2_array       = vld1q_u16(y_pre + k);
            uint32x4_t       frame2_array_u32_1 = vmovl_u16(vget_low_u16(frame2_array));
            uint32x4_t       frame2_array_u32_2 = vmovl_u16(vget_high_u16(frame2_array));
            frame2_array_u32_1                  = vmulq_u32(frame2_array_u32_1,
                                           adjusted_weight_int32[subblock_idx_h + (j >= block_width / 2)]);
            frame2_array_u32_2                  = vmulq_u32(frame2_array_u32_2,
                                           adjusted_weight_int32[subblock_idx_h + (j >= block_width / 2)]);

            accumulator_array1 = vaddq_u32(accumulator_array1, frame2_array_u32_1);
            accumulator_array2 = vaddq_u32(accumulator_array2, frame2_array_u32_2);
            vst1q_u32(y_accum + k, accumulator_array1);
            vst1q_u32(y_accum + k + 4, accumulator_array2);
        }
    }
}

static void apply_filtering_central_loop_hbd(uint16_t w, uint16_t h, uint16_t *src, uint16_t src_stride,
                                             uint32_t *accum, uint16_t *count) {
    assert(w % 8 == 0);

    uint32x4_t modifier       = vdupq_n_u32(TF_PLANEWISE_FILTER_WEIGHT_SCALE);
    uint16x8_t modifier_epi16 = vdupq_n_u16(TF_PLANEWISE_FILTER_WEIGHT_SCALE);

    for (uint16_t k = 0, i = 0; i < h; i++) {
        for (uint16_t j = 0; j < w; j += 8) {
            const uint32x4_t src_1 = vmovl_u16(vld1_u16(src + i * src_stride + j + 0));
            const uint32x4_t src_2 = vmovl_u16(vld1_u16(src + i * src_stride + j + 4));

            vst1q_u32(accum + k + 0, vmulq_u32(modifier, src_1));
            vst1q_u32(accum + k + 4, vmulq_u32(modifier, src_2));
            vst1q_u16(count + k, modifier_epi16);

            k += 8;
        }
    }
}

void svt_aom_apply_filtering_central_neon(struct MeContext *me_ctx, EbPictureBufferDesc *input_picture_ptr_central,
                                          EbByte *src, uint32_t **accum, uint16_t **count, uint16_t blk_width,
                                          uint16_t blk_height, uint32_t ss_x, uint32_t ss_y) {
    uint16_t src_stride_y = input_picture_ptr_central->stride_y;

    // Luma
    apply_filtering_central_loop_lbd(blk_width, blk_height, src[C_Y], src_stride_y, accum[C_Y], count[C_Y]);

    // Chroma
    if (me_ctx->tf_chroma) {
        uint16_t blk_height_ch = blk_height >> ss_y;
        uint16_t blk_width_ch  = blk_width >> ss_x;
        uint16_t src_stride_ch = src_stride_y >> ss_x;
        apply_filtering_central_loop_lbd(blk_width_ch, blk_height_ch, src[C_U], src_stride_ch, accum[C_U], count[C_U]);
        apply_filtering_central_loop_lbd(blk_width_ch, blk_height_ch, src[C_V], src_stride_ch, accum[C_V], count[C_V]);
    }
}

void svt_aom_apply_filtering_central_highbd_neon(struct MeContext    *me_ctx,
                                                 EbPictureBufferDesc *input_picture_ptr_central, uint16_t **src_16bit,
                                                 uint32_t **accum, uint16_t **count, uint16_t blk_width,
                                                 uint16_t blk_height, uint32_t ss_x, uint32_t ss_y) {
    uint16_t src_stride_y = input_picture_ptr_central->stride_y;

    // Luma
    apply_filtering_central_loop_hbd(blk_width, blk_height, src_16bit[C_Y], src_stride_y, accum[C_Y], count[C_Y]);

    // Chroma
    if (me_ctx->tf_chroma) {
        uint16_t blk_height_ch = blk_height >> ss_y;
        uint16_t blk_width_ch  = blk_width >> ss_x;
        uint16_t src_stride_ch = src_stride_y >> ss_x;
        apply_filtering_central_loop_hbd(
            blk_width_ch, blk_height_ch, src_16bit[C_U], src_stride_ch, accum[C_U], count[C_U]);
        apply_filtering_central_loop_hbd(
            blk_width_ch, blk_height_ch, src_16bit[C_V], src_stride_ch, accum[C_V], count[C_V]);
    }
}

void svt_av1_apply_temporal_filter_planewise_medium_hbd_neon(
    struct MeContext *me_ctx, const uint16_t *y_src, int y_src_stride, const uint16_t *y_pre, int y_pre_stride,
    const uint16_t *u_src, const uint16_t *v_src, int uv_src_stride, const uint16_t *u_pre, const uint16_t *v_pre,
    int uv_pre_stride, unsigned int block_width, unsigned int block_height, int ss_x, int ss_y, uint32_t *y_accum,
    uint16_t *y_count, uint32_t *u_accum, uint16_t *u_count, uint32_t *v_accum, uint16_t *v_count,
    uint32_t encoder_bit_depth) {
    uint32_t luma_window_error_quad_fp8[4];

    svt_av1_apply_temporal_filter_planewise_medium_hbd_partial_neon(me_ctx,
                                                                    y_src,
                                                                    y_src_stride,
                                                                    y_pre,
                                                                    y_pre_stride,
                                                                    (unsigned int)block_width,
                                                                    (unsigned int)block_height,
                                                                    y_accum,
                                                                    y_count,
                                                                    me_ctx->tf_decay_factor_fp16[C_Y],
                                                                    luma_window_error_quad_fp8,
                                                                    0,
                                                                    encoder_bit_depth);
    if (me_ctx->tf_chroma) {
        svt_av1_apply_temporal_filter_planewise_medium_hbd_partial_neon(me_ctx,
                                                                        u_src,
                                                                        uv_src_stride,
                                                                        u_pre,
                                                                        uv_pre_stride,
                                                                        (unsigned int)block_width >> ss_x,
                                                                        (unsigned int)block_height >> ss_y,
                                                                        u_accum,
                                                                        u_count,
                                                                        me_ctx->tf_decay_factor_fp16[C_U],
                                                                        luma_window_error_quad_fp8,
                                                                        1,
                                                                        encoder_bit_depth);

        svt_av1_apply_temporal_filter_planewise_medium_hbd_partial_neon(me_ctx,
                                                                        v_src,
                                                                        uv_src_stride,
                                                                        v_pre,
                                                                        uv_pre_stride,
                                                                        (unsigned int)block_width >> ss_x,
                                                                        (unsigned int)block_height >> ss_y,
                                                                        v_accum,
                                                                        v_count,
                                                                        me_ctx->tf_decay_factor_fp16[C_V],
                                                                        luma_window_error_quad_fp8,
                                                                        1,
                                                                        encoder_bit_depth);
    }
}

int32_t svt_estimate_noise_highbd_fp16_neon(const uint16_t *src, int width, int height, int stride, int bd) {
    int64_t sum = 0;
    int64_t num = 0;

    //  A | B | C
    //  D | E | F
    //  G | H | I
    // g_x = (A - I) + (G - C) + 2*(D - F)
    // g_y = (A - I) - (G - C) + 2*(B - H)
    // v   = 4*E - 2*(D+F+B+H) + (A+C+G+I)

    const int16x8_t edge_treshold      = vdupq_n_s16(EDGE_THRESHOLD);
    const int32x4_t zero               = vdupq_n_s32(0);
    int32x4_t       num_accumulator_lo = zero;
    int32x4_t       num_accumulator_hi = zero;
    int32x4_t       sum_accumulator_lo = zero;
    int32x4_t       sum_accumulator_hi = zero;
    const int16x8_t shift              = vdupq_n_s16(8 - bd);

    for (int i = 1; i < height - 1; ++i) {
        int j = 1;
        for (; j + 16 < width - 1; j += 16) {
            const int k = i * stride + j;

            const int16x8_t A_lo = vreinterpretq_s16_u16(vld1q_u16(&src[k - stride - 1]));
            const int16x8_t A_hi = vreinterpretq_s16_u16(vld1q_u16(&src[k - stride + 7]));
            const int16x8_t B_lo = vreinterpretq_s16_u16(vld1q_u16(&src[k - stride]));
            const int16x8_t B_hi = vreinterpretq_s16_u16(vld1q_u16(&src[k - stride + 8]));
            const int16x8_t C_lo = vreinterpretq_s16_u16(vld1q_u16(&src[k - stride + 1]));
            const int16x8_t C_hi = vreinterpretq_s16_u16(vld1q_u16(&src[k - stride + 9]));
            const int16x8_t D_lo = vreinterpretq_s16_u16(vld1q_u16(&src[k - 1]));
            const int16x8_t D_hi = vreinterpretq_s16_u16(vld1q_u16(&src[k + 7]));
            const int16x8_t E_lo = vreinterpretq_s16_u16(vld1q_u16(&src[k]));
            const int16x8_t E_hi = vreinterpretq_s16_u16(vld1q_u16(&src[k + 8]));
            const int16x8_t F_lo = vreinterpretq_s16_u16(vld1q_u16(&src[k + 1]));
            const int16x8_t F_hi = vreinterpretq_s16_u16(vld1q_u16(&src[k + 9]));
            const int16x8_t G_lo = vreinterpretq_s16_u16(vld1q_u16(&src[k + stride - 1]));
            const int16x8_t G_hi = vreinterpretq_s16_u16(vld1q_u16(&src[k + stride + 7]));
            const int16x8_t H_lo = vreinterpretq_s16_u16(vld1q_u16(&src[k + stride]));
            const int16x8_t H_hi = vreinterpretq_s16_u16(vld1q_u16(&src[k + stride + 8]));
            const int16x8_t I_lo = vreinterpretq_s16_u16(vld1q_u16(&src[k + stride + 1]));
            const int16x8_t I_hi = vreinterpretq_s16_u16(vld1q_u16(&src[k + stride + 9]));

            const int16x8_t A_m_I_lo   = vsubq_s16(A_lo, I_lo);
            const int16x8_t A_m_I_hi   = vsubq_s16(A_hi, I_hi);
            const int16x8_t G_m_C_lo   = vsubq_s16(G_lo, C_lo);
            const int16x8_t G_m_C_hi   = vsubq_s16(G_hi, C_hi);
            const int16x8_t D_m_Fx2_lo = vshlq_n_s16(vsubq_s16(D_lo, F_lo), 1);
            const int16x8_t D_m_Fx2_hi = vshlq_n_s16(vsubq_s16(D_hi, F_hi), 1);
            const int16x8_t B_m_Hx2_lo = vshlq_n_s16(vsubq_s16(B_lo, H_lo), 1);
            const int16x8_t B_m_Hx2_hi = vshlq_n_s16(vsubq_s16(B_hi, H_hi), 1);

            const int16x8_t gx_256_lo = vabsq_s16(vaddq_s16(vaddq_s16(A_m_I_lo, G_m_C_lo), D_m_Fx2_lo));
            const int16x8_t gx_256_hi = vabsq_s16(vaddq_s16(vaddq_s16(A_m_I_hi, G_m_C_hi), D_m_Fx2_hi));
            const int16x8_t gy_256_lo = vabdq_s16(vaddq_s16(B_m_Hx2_lo, A_m_I_lo), G_m_C_lo);
            const int16x8_t gy_256_hi = vabdq_s16(vaddq_s16(B_m_Hx2_hi, A_m_I_hi), G_m_C_hi);
            const int16x8_t ga_256_lo = vrshlq_s16(vaddq_s16(gx_256_lo, gy_256_lo), shift);
            const int16x8_t ga_256_hi = vrshlq_s16(vaddq_s16(gx_256_hi, gy_256_hi), shift);

            const int16x8_t D_F_B_Hx2_lo = vshlq_n_s16(vaddq_s16(vaddq_s16(D_lo, F_lo), vaddq_s16(B_lo, H_lo)), 1);
            const int16x8_t D_F_B_Hx2_hi = vshlq_n_s16(vaddq_s16(vaddq_s16(D_hi, F_hi), vaddq_s16(B_hi, H_hi)), 1);
            const int16x8_t A_C_G_I_lo   = vaddq_s16(vaddq_s16(A_lo, C_lo), vaddq_s16(G_lo, I_lo));
            const int16x8_t A_C_G_I_hi   = vaddq_s16(vaddq_s16(A_hi, C_hi), vaddq_s16(G_hi, I_hi));
            int16x8_t       v_256_lo     = vabdq_s16(vaddq_s16(A_C_G_I_lo, vshlq_n_s16(E_lo, 2)), D_F_B_Hx2_lo);
            int16x8_t       v_256_hi     = vabdq_s16(vaddq_s16(A_C_G_I_hi, vshlq_n_s16(E_hi, 2)), D_F_B_Hx2_hi);

            //if (ga < EDGE_THRESHOLD)
            const int16x8_t cmp_lo = vreinterpretq_s16_u16(vshrq_n_u16(vcgtq_s16(edge_treshold, ga_256_lo), 15));
            const int16x8_t cmp_hi = vreinterpretq_s16_u16(vshrq_n_u16(vcgtq_s16(edge_treshold, ga_256_hi), 15));

            v_256_lo = vrshlq_s16(vmulq_s16(v_256_lo, cmp_lo), shift);
            v_256_hi = vrshlq_s16(vmulq_s16(v_256_hi, cmp_hi), shift);

            //num_accumulator and sum_accumulator have 32bit values
            num_accumulator_lo = vaddq_s32(
                num_accumulator_lo, vaddq_s32(vmovl_s16(vget_low_s16(cmp_lo)), vmovl_s16(vget_high_s16(cmp_lo))));
            num_accumulator_hi = vaddq_s32(
                num_accumulator_hi, vaddq_s32(vmovl_s16(vget_low_s16(cmp_hi)), vmovl_s16(vget_high_s16(cmp_hi))));
            sum_accumulator_lo = vaddq_s32(
                sum_accumulator_lo, vaddq_s32(vmovl_s16(vget_low_s16(v_256_lo)), vmovl_s16(vget_high_s16(v_256_lo))));
            sum_accumulator_hi = vaddq_s32(
                sum_accumulator_hi, vaddq_s32(vmovl_s16(vget_low_s16(v_256_hi)), vmovl_s16(vget_high_s16(v_256_hi))));
        }
        for (; j < width - 1; ++j) {
            const int k = i * stride + j;

            // Sobel gradients
            const int g_x = (src[k - stride - 1] - src[k - stride + 1]) + (src[k + stride - 1] - src[k + stride + 1]) +
                2 * (src[k - 1] - src[k + 1]);
            const int g_y = (src[k - stride - 1] - src[k + stride - 1]) + (src[k - stride + 1] - src[k + stride + 1]) +
                2 * (src[k - stride] - src[k + stride]);
            const int ga = ROUND_POWER_OF_TWO(abs(g_x) + abs(g_y),
                                              bd - 8); // divide by 2^2 and round up
            if (ga < EDGE_THRESHOLD) { // Do not consider edge pixels to estimate the noise
                // Find Laplacian
                const int v = 4 * src[k] - 2 * (src[k - 1] + src[k + 1] + src[k - stride] + src[k + stride]) +
                    (src[k - stride - 1] + src[k - stride + 1] + src[k + stride - 1] + src[k + stride + 1]);
                sum += ROUND_POWER_OF_TWO(abs(v), bd - 8);
                ++num;
            }
        }
    }

    int32x4_t sum_256_lo = vpaddq_s32(sum_accumulator_lo, num_accumulator_lo);
    int32x4_t sum_256_hi = vpaddq_s32(sum_accumulator_hi, num_accumulator_hi);
    sum_256_lo           = vpaddq_s32(sum_256_lo, sum_256_lo);
    sum_256_hi           = vpaddq_s32(sum_256_hi, sum_256_hi);
    sum_256_lo           = vaddq_s32(sum_256_lo, sum_256_hi);
    sum += vgetq_lane_s32(sum_256_lo, 0);
    num += vgetq_lane_s32(vextq_s32(sum_256_lo, zero, 1), 0);

    // If very few smooth pels, return -1 since the estimate is unreliable
    if (num < SMOOTH_THRESHOLD) {
        return -65536 /*-1:fp16*/;
    }

    FP_ASSERT((((int64_t)sum * SQRT_PI_BY_2_FP16) / (6 * num)) < ((int64_t)1 << 31));
    return (int32_t)((sum * SQRT_PI_BY_2_FP16) / (6 * num));
}
