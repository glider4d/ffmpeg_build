/*
 * copyright (c) 2000,2001 Fabrice Bellard
 * H.263+ support
 * copyright (c) 2002-2004 Michael Niedermayer <michaelni@gmx.at>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * MPEG-4 tables.
 */

#ifndef AVCODEC_MPEG4DATA_H
#define AVCODEC_MPEG4DATA_H

#include "mpeg4videodata.h"

/* dc encoding for MPEG-4 */
const uint8_t ff_mpeg4_DCtab_lum[13][2] =
{
    {3,3}, {3,2}, {2,2}, {2,3}, {1,3}, {1,4}, {1,5}, {1,6}, {1,7},
    {1,8}, {1,9}, {1,10}, {1,11},
};

const uint8_t ff_mpeg4_DCtab_chrom[13][2] =
{
    {3,2}, {2,2}, {1,2}, {1,3}, {1,4}, {1,5}, {1,6}, {1,7}, {1,8},
    {1,9}, {1,10}, {1,11}, {1,12},
};

const uint16_t ff_mpeg4_intra_vlc[103][2] = {
{ 0x2, 2 },
{ 0x6, 3 },{ 0xf, 4 },{ 0xd, 5 },{ 0xc, 5 },
{ 0x15, 6 },{ 0x13, 6 },{ 0x12, 6 },{ 0x17, 7 },
{ 0x1f, 8 },{ 0x1e, 8 },{ 0x1d, 8 },{ 0x25, 9 },
{ 0x24, 9 },{ 0x23, 9 },{ 0x21, 9 },{ 0x21, 10 },
{ 0x20, 10 },{ 0xf, 10 },{ 0xe, 10 },{ 0x7, 11 },
{ 0x6, 11 },{ 0x20, 11 },{ 0x21, 11 },{ 0x50, 12 },
{ 0x51, 12 },{ 0x52, 12 },{ 0xe, 4 },{ 0x14, 6 },
{ 0x16, 7 },{ 0x1c, 8 },{ 0x20, 9 },{ 0x1f, 9 },
{ 0xd, 10 },{ 0x22, 11 },{ 0x53, 12 },{ 0x55, 12 },
{ 0xb, 5 },{ 0x15, 7 },{ 0x1e, 9 },{ 0xc, 10 },
{ 0x56, 12 },{ 0x11, 6 },{ 0x1b, 8 },{ 0x1d, 9 },
{ 0xb, 10 },{ 0x10, 6 },{ 0x22, 9 },{ 0xa, 10 },
{ 0xd, 6 },{ 0x1c, 9 },{ 0x8, 10 },{ 0x12, 7 },
{ 0x1b, 9 },{ 0x54, 12 },{ 0x14, 7 },{ 0x1a, 9 },
{ 0x57, 12 },{ 0x19, 8 },{ 0x9, 10 },{ 0x18, 8 },
{ 0x23, 11 },{ 0x17, 8 },{ 0x19, 9 },{ 0x18, 9 },
{ 0x7, 10 },{ 0x58, 12 },{ 0x7, 4 },{ 0xc, 6 },
{ 0x16, 8 },{ 0x17, 9 },{ 0x6, 10 },{ 0x5, 11 },
{ 0x4, 11 },{ 0x59, 12 },{ 0xf, 6 },{ 0x16, 9 },
{ 0x5, 10 },{ 0xe, 6 },{ 0x4, 10 },{ 0x11, 7 },
{ 0x24, 11 },{ 0x10, 7 },{ 0x25, 11 },{ 0x13, 7 },
{ 0x5a, 12 },{ 0x15, 8 },{ 0x5b, 12 },{ 0x14, 8 },
{ 0x13, 8 },{ 0x1a, 8 },{ 0x15, 9 },{ 0x14, 9 },
{ 0x13, 9 },{ 0x12, 9 },{ 0x11, 9 },{ 0x26, 11 },
{ 0x27, 11 },{ 0x5c, 12 },{ 0x5d, 12 },{ 0x5e, 12 },
{ 0x5f, 12 },{ 0x3, 7 },
};

const int8_t ff_mpeg4_intra_level[102] = {
  1,  2,  3,  4,  5,  6,  7,  8,
  9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24,
 25, 26, 27,  1,  2,  3,  4,  5,
  6,  7,  8,  9, 10,  1,  2,  3,
  4,  5,  1,  2,  3,  4,  1,  2,
  3,  1,  2,  3,  1,  2,  3,  1,
  2,  3,  1,  2,  1,  2,  1,  1,
  1,  1,  1,  1,  2,  3,  4,  5,
  6,  7,  8,  1,  2,  3,  1,  2,
  1,  2,  1,  2,  1,  2,  1,  2,
  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,
};

const int8_t ff_mpeg4_intra_run[102] = {
  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  2,  2,  2,
  2,  2,  3,  3,  3,  3,  4,  4,
  4,  5,  5,  5,  6,  6,  6,  7,
  7,  7,  8,  8,  9,  9, 10, 11,
 12, 13, 14,  0,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  1,  2,  2,
  3,  3,  4,  4,  5,  5,  6,  6,
  7,  8,  9, 10, 11, 12, 13, 14,
 15, 16, 17, 18, 19, 20,
};

RLTable ff_mpeg4_rl_intra = {
    102,
    67,
    ff_mpeg4_intra_vlc,
    ff_mpeg4_intra_run,
    ff_mpeg4_intra_level,
};

/* Note this is identical to the intra rvlc except that it is reordered. */
static const uint16_t inter_rvlc[170][2]={
{0x0006,  3},{0x0001,  4},{0x0004,  5},{0x001C,  7},
{0x003C,  8},{0x003D,  8},{0x007C,  9},{0x00FC, 10},
{0x00FD, 10},{0x01FC, 11},{0x01FD, 11},{0x03FC, 12},
{0x07FC, 13},{0x07FD, 13},{0x0BFC, 13},{0x0BFD, 13},
{0x0FFC, 14},{0x0FFD, 14},{0x1FFC, 15},{0x0007,  3},
{0x000C,  6},{0x005C,  8},{0x007D,  9},{0x017C, 10},
{0x02FC, 11},{0x03FD, 12},{0x0DFC, 13},{0x17FC, 14},
{0x17FD, 14},{0x000A,  4},{0x001D,  7},{0x00BC,  9},
{0x02FD, 11},{0x05FC, 12},{0x1BFC, 14},{0x1BFD, 14},
{0x0005,  5},{0x005D,  8},{0x017D, 10},{0x05FD, 12},
{0x0DFD, 13},{0x1DFC, 14},{0x1FFD, 15},{0x0008,  5},
{0x006C,  8},{0x037C, 11},{0x0EFC, 13},{0x2FFC, 15},
{0x0009,  5},{0x00BD,  9},{0x037D, 11},{0x0EFD, 13},
{0x000D,  6},{0x01BC, 10},{0x06FC, 12},{0x1DFD, 14},
{0x0014,  6},{0x01BD, 10},{0x06FD, 12},{0x2FFD, 15},
{0x0015,  6},{0x01DC, 10},{0x0F7C, 13},{0x002C,  7},
{0x01DD, 10},{0x1EFC, 14},{0x002D,  7},{0x03BC, 11},
{0x0034,  7},{0x077C, 12},{0x006D,  8},{0x0F7D, 13},
{0x0074,  8},{0x1EFD, 14},{0x0075,  8},{0x1F7C, 14},
{0x00DC,  9},{0x1F7D, 14},{0x00DD,  9},{0x1FBC, 14},
{0x00EC,  9},{0x37FC, 15},{0x01EC, 10},{0x01ED, 10},
{0x01F4, 10},{0x03BD, 11},{0x03DC, 11},{0x03DD, 11},
{0x03EC, 11},{0x03ED, 11},{0x03F4, 11},{0x077D, 12},
{0x07BC, 12},{0x07BD, 12},{0x0FBC, 13},{0x0FBD, 13},
{0x0FDC, 13},{0x0FDD, 13},{0x1FBD, 14},{0x1FDC, 14},
{0x1FDD, 14},{0x37FD, 15},{0x3BFC, 15},
{0x000B,  4},{0x0078,  8},{0x03F5, 11},{0x0FEC, 13},
{0x1FEC, 14},{0x0012,  5},{0x00ED,  9},{0x07DC, 12},
{0x1FED, 14},{0x3BFD, 15},{0x0013,  5},{0x03F8, 11},
{0x3DFC, 15},{0x0018,  6},{0x07DD, 12},{0x0019,  6},
{0x07EC, 12},{0x0022,  6},{0x0FED, 13},{0x0023,  6},
{0x0FF4, 13},{0x0035,  7},{0x0FF5, 13},{0x0038,  7},
{0x0FF8, 13},{0x0039,  7},{0x0FF9, 13},{0x0042,  7},
{0x1FF4, 14},{0x0043,  7},{0x1FF5, 14},{0x0079,  8},
{0x1FF8, 14},{0x0082,  8},{0x3DFD, 15},{0x0083,  8},
{0x00F4,  9},{0x00F5,  9},{0x00F8,  9},{0x00F9,  9},
{0x0102,  9},{0x0103,  9},{0x01F5, 10},{0x01F8, 10},
{0x01F9, 10},{0x0202, 10},{0x0203, 10},{0x03F9, 11},
{0x0402, 11},{0x0403, 11},{0x07ED, 12},{0x07F4, 12},
{0x07F5, 12},{0x07F8, 12},{0x07F9, 12},{0x0802, 12},
{0x0803, 12},{0x1002, 13},{0x1003, 13},{0x1FF9, 14},
{0x2002, 14},{0x2003, 14},{0x3EFC, 15},{0x3EFD, 15},
{0x3F7C, 15},{0x3F7D, 15},{0x0000, 4}
};

static const int8_t inter_rvlc_run[169]={
 0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  1,  1,  1,  1,  1,
 1,  1,  1,  1,  1,  2,  2,  2,
 2,  2,  2,  2,  3,  3,  3,  3,
 3,  3,  3,  4,  4,  4,  4,  4,
 5,  5,  5,  5,  6,  6,  6,  6,
 7,  7,  7,  7,  8,  8,  8,  9,
 9,  9, 10, 10, 11, 11, 12, 12,
13, 13, 14, 14, 15, 15, 16, 16,
17, 17, 18, 19, 20, 21, 22, 23,
24, 25, 26, 27, 28, 29, 30, 31,
32, 33, 34, 35, 36, 37, 38,
 0,  0,  0,  0,  0,  1,  1,  1,
 1,  1,  2,  2,  2,  3,  3,  4,
 4,  5,  5,  6,  6,  7,  7,  8,
 8,  9,  9, 10, 10, 11, 11, 12,
12, 13, 13, 14, 15, 16, 17, 18,
19, 20, 21, 22, 23, 24, 25, 26,
27, 28, 29, 30, 31, 32, 33, 34,
35, 36, 37, 38, 39, 40, 41, 42,
43, 44,
};

static const int8_t inter_rvlc_level[169]={
 1,  2,  3,  4,  5,  6,  7,  8,
 9, 10, 11, 12, 13, 14, 15, 16,
17, 18, 19,  1,  2,  3,  4,  5,
 6,  7,  8,  9, 10,  1,  2,  3,
 4,  5,  6,  7,  1,  2,  3,  4,
 5,  6,  7,  1,  2,  3,  4,  5,
 1,  2,  3,  4,  1,  2,  3,  4,
 1,  2,  3,  4,  1,  2,  3,  1,
 2,  3,  1,  2,  1,  2,  1,  2,
 1,  2,  1,  2,  1,  2,  1,  2,
 1,  2,  1,  1,  1,  1,  1,  1,
 1,  1,  1,  1,  1,  1,  1,  1,
 1,  1,  1,  1,  1,  1,  1,
 1,  2,  3,  4,  5,  1,  2,  3,
 4,  5,  1,  2,  3,  1,  2,  1,
 2,  1,  2,  1,  2,  1,  2,  1,
 2,  1,  2,  1,  2,  1,  2,  1,
 2,  1,  2,  1,  1,  1,  1,  1,
 1,  1,  1,  1,  1,  1,  1,  1,
 1,  1,  1,  1,  1,  1,  1,  1,
 1,  1,  1,  1,  1,  1,  1,  1,
 1,  1,
};

RLTable ff_rvlc_rl_inter = {
    169,
    103,
    inter_rvlc,
    inter_rvlc_run,
    inter_rvlc_level,
};

static const uint16_t intra_rvlc[170][2]={
{0x0006,  3},{0x0007,  3},{0x000A,  4},{0x0009,  5},
{0x0014,  6},{0x0015,  6},{0x0034,  7},{0x0074,  8},
{0x0075,  8},{0x00DD,  9},{0x00EC,  9},{0x01EC, 10},
{0x01ED, 10},{0x01F4, 10},{0x03EC, 11},{0x03ED, 11},
{0x03F4, 11},{0x077D, 12},{0x07BC, 12},{0x0FBD, 13},
{0x0FDC, 13},{0x07BD, 12},{0x0FDD, 13},{0x1FBD, 14},
{0x1FDC, 14},{0x1FDD, 14},{0x1FFC, 15},{0x0001,  4},
{0x0008,  5},{0x002D,  7},{0x006C,  8},{0x006D,  8},
{0x00DC,  9},{0x01DD, 10},{0x03DC, 11},{0x03DD, 11},
{0x077C, 12},{0x0FBC, 13},{0x1F7D, 14},{0x1FBC, 14},
{0x0004,  5},{0x002C,  7},{0x00BC,  9},{0x01DC, 10},
{0x03BC, 11},{0x03BD, 11},{0x0EFD, 13},{0x0F7C, 13},
{0x0F7D, 13},{0x1EFD, 14},{0x1F7C, 14},{0x0005,  5},
{0x005C,  8},{0x00BD,  9},{0x037D, 11},{0x06FC, 12},
{0x0EFC, 13},{0x1DFD, 14},{0x1EFC, 14},{0x1FFD, 15},
{0x000C,  6},{0x005D,  8},{0x01BD, 10},{0x03FD, 12},
{0x06FD, 12},{0x1BFD, 14},{0x000D,  6},{0x007D,  9},
{0x02FC, 11},{0x05FC, 12},{0x1BFC, 14},{0x1DFC, 14},
{0x001C,  7},{0x017C, 10},{0x02FD, 11},{0x05FD, 12},
{0x2FFC, 15},{0x001D,  7},{0x017D, 10},{0x037C, 11},
{0x0DFD, 13},{0x2FFD, 15},{0x003C,  8},{0x01BC, 10},
{0x0BFD, 13},{0x17FD, 14},{0x003D,  8},{0x01FD, 11},
{0x0DFC, 13},{0x37FC, 15},{0x007C,  9},{0x03FC, 12},
{0x00FC, 10},{0x0BFC, 13},{0x00FD, 10},{0x37FD, 15},
{0x01FC, 11},{0x07FC, 13},{0x07FD, 13},{0x0FFC, 14},
{0x0FFD, 14},{0x17FC, 14},{0x3BFC, 15},
{0x000B,  4},{0x0078,  8},{0x03F5, 11},{0x0FEC, 13},
{0x1FEC, 14},{0x0012,  5},{0x00ED,  9},{0x07DC, 12},
{0x1FED, 14},{0x3BFD, 15},{0x0013,  5},{0x03F8, 11},
{0x3DFC, 15},{0x0018,  6},{0x07DD, 12},{0x0019,  6},
{0x07EC, 12},{0x0022,  6},{0x0FED, 13},{0x0023,  6},
{0x0FF4, 13},{0x0035,  7},{0x0FF5, 13},{0x0038,  7},
{0x0FF8, 13},{0x0039,  7},{0x0FF9, 13},{0x0042,  7},
{0x1FF4, 14},{0x0043,  7},{0x1FF5, 14},{0x0079,  8},
{0x1FF8, 14},{0x0082,  8},{0x3DFD, 15},{0x0083,  8},
{0x00F4,  9},{0x00F5,  9},{0x00F8,  9},{0x00F9,  9},
{0x0102,  9},{0x0103,  9},{0x01F5, 10},{0x01F8, 10},
{0x01F9, 10},{0x0202, 10},{0x0203, 10},{0x03F9, 11},
{0x0402, 11},{0x0403, 11},{0x07ED, 12},{0x07F4, 12},
{0x07F5, 12},{0x07F8, 12},{0x07F9, 12},{0x0802, 12},
{0x0803, 12},{0x1002, 13},{0x1003, 13},{0x1FF9, 14},
{0x2002, 14},{0x2003, 14},{0x3EFC, 15},{0x3EFD, 15},
{0x3F7C, 15},{0x3F7D, 15},{0x0000,  4}
};

static const int8_t intra_rvlc_run[169]={
 0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  1,  1,  1,  1,  1,
 1,  1,  1,  1,  1,  1,  1,  1,
 2,  2,  2,  2,  2,  2,  2,  2,
 2,  2,  2,  3,  3,  3,  3,  3,
 3,  3,  3,  3,  4,  4,  4,  4,
 4,  4,  5,  5,  5,  5,  5,  5,
 6,  6,  6,  6,  6,  7,  7,  7,
 7,  7,  8,  8,  8,  8,  9,  9,
 9,  9, 10, 10, 11, 11, 12, 12,
13, 14, 15, 16, 17, 18, 19,
 0,  0,  0,  0,  0,  1,  1,  1,
 1,  1,  2,  2,  2,  3,  3,  4,
 4,  5,  5,  6,  6,  7,  7,  8,
 8,  9,  9, 10, 10, 11, 11, 12,
12, 13, 13, 14, 15, 16, 17, 18,
19, 20, 21, 22, 23, 24, 25, 26,
27, 28, 29, 30, 31, 32, 33, 34,
35, 36, 37, 38, 39, 40, 41, 42,
43, 44,
};

static const int8_t intra_rvlc_level[169]={
 1,  2,  3,  4,  5,  6,  7,  8,
 9, 10, 11, 12, 13, 14, 15, 16,
17, 18, 19, 20, 21, 22, 23, 24,
25, 26, 27,  1,  2,  3,  4,  5,
 6,  7,  8,  9, 10, 11, 12, 13,
 1,  2,  3,  4,  5,  6,  7,  8,
 9, 10, 11,  1,  2,  3,  4,  5,
 6,  7,  8,  9,  1,  2,  3,  4,
 5,  6,  1,  2,  3,  4,  5,  6,
 1,  2,  3,  4,  5,  1,  2,  3,
 4,  5,  1,  2,  3,  4,  1,  2,
 3,  4,  1,  2,  1,  2,  1,  2,
 1,  1,  1,  1,  1,  1,  1,
 1,  2,  3,  4,  5,  1,  2,  3,
 4,  5,  1,  2,  3,  1,  2,  1,
 2,  1,  2,  1,  2,  1,  2,  1,
 2,  1,  2,  1,  2,  1,  2,  1,
 2,  1,  2,  1,  1,  1,  1,  1,
 1,  1,  1,  1,  1,  1,  1,  1,
 1,  1,  1,  1,  1,  1,  1,  1,
 1,  1,  1,  1,  1,  1,  1,  1,
 1,  1,
};

RLTable ff_rvlc_rl_intra = {
    169,
    103,
    intra_rvlc,
    intra_rvlc_run,
    intra_rvlc_level,
};

const uint8_t ff_sprite_trajectory_lens[15] = {
    2,  3,  3,  3,  3,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
};

const uint8_t ff_mb_type_b_tab[4][2] = {
 {1, 1}, {1, 2}, {1, 3}, {1, 4},
};

/* these matrixes will be permuted for the idct */
const int16_t ff_mpeg4_default_intra_matrix[64] = {
  8, 17, 18, 19, 21, 23, 25, 27,
 17, 18, 19, 21, 23, 25, 27, 28,
 20, 21, 22, 23, 24, 26, 28, 30,
 21, 22, 23, 24, 26, 28, 30, 32,
 22, 23, 24, 26, 28, 30, 32, 35,
 23, 24, 26, 28, 30, 32, 35, 38,
 25, 26, 28, 30, 32, 35, 38, 41,
 27, 28, 30, 32, 35, 38, 41, 45,
};

const int16_t ff_mpeg4_default_non_intra_matrix[64] = {
 16, 17, 18, 19, 20, 21, 22, 23,
 17, 18, 19, 20, 21, 22, 23, 24,
 18, 19, 20, 21, 22, 23, 24, 25,
 19, 20, 21, 22, 23, 24, 26, 27,
 20, 21, 22, 23, 25, 26, 27, 28,
 21, 22, 23, 24, 26, 27, 28, 30,
 22, 23, 24, 26, 27, 28, 30, 31,
 23, 24, 25, 27, 28, 30, 31, 33,
};

const uint8_t ff_mpeg4_y_dc_scale_table[32]={
//  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
    0, 8, 8, 8, 8,10,12,14,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,34,36,38,40,42,44,46
};
const uint8_t ff_mpeg4_c_dc_scale_table[32]={
//  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
    0, 8, 8, 8, 8, 9, 9,10,10,11,11,12,12,13,13,14,14,15,15,16,16,17,17,18,18,19,20,21,22,23,24,25
};

const uint8_t ff_mpeg4_dc_threshold[8]={
    99, 13, 15, 17, 19, 21, 23, 0
};

/* Note these are different in studio mode */
const uint8_t ff_mpeg4_studio_dc_luma[19][2] = {
    {  2,  4 }, { 10,  4 }, {  3,  4 }, {  1,  5 }, {  0,  6 }, { 11,  7 },
    { 12,  8 }, { 13,  9 }, { 14, 10 }, { 15, 11 }, { 16, 12 }, { 17, 13 },
    { 18, 13 }, {  7,  3 }, {  6,  3 }, {  8,  3 }, {  5,  3 }, {  9,  3 },
    {  4,  3 },
};

const uint8_t ff_mpeg4_studio_dc_chroma[19][2] = {
    {  0,  4 }, {  8,  4 }, {  1,  4 }, {  9,  5 }, { 10,  6 }, { 11,  7 },
    { 12,  8 }, { 13,  9 }, { 14, 10 }, { 15, 11 }, { 16, 12 }, { 17, 13 },
    { 18, 13 }, {  5,  3 }, {  4,  3 }, {  6,  3 }, {  3,  3 }, {  7,  3 },
    {  2,  3 },
};

const uint8_t ff_mpeg4_studio_intra[12][24][2] = {
    {
        {  0, -6 }, { 21, 13 }, {  6, 13 }, {  5, 12 }, {  4, 11 }, { 20, 10 },
        {  3,  9 }, { 12,  8 }, { 11,  7 }, { 10,  7 }, {  2,  7 }, { 19,  6 },
        { 18,  6 }, {  9,  6 }, {  8,  5 }, { 17,  4 }, {  7,  4 }, {  1,  4 },
        {  0,  4 }, { 16,  3 }, { 15,  3 }, { 14,  3 }, { 13,  2 },
    },
    {
        {  0, -6 }, { 21,  8 }, { 20,  8 }, { 19,  7 }, { 18,  5 }, { 17,  4 },
        { 16,  3 }, { 15,  2 }, { 14,  1 },
    },
    {
        {  0, -6 }, {  0,-15 }, { 20, 15 }, { 19, 14 }, {  6, 14 }, {  5, 14 },
        { 21, 13 }, { 18, 13 }, { 17, 11 }, { 12, 10 }, {  4,  9 }, { 16,  8 },
        {  3,  7 }, { 15,  6 }, { 11,  6 }, {  2,  5 }, {  1,  5 }, { 10,  4 },
        {  9,  4 }, { 14,  3 }, {  8,  3 }, {  7,  3 }, {  0,  3 }, { 13,  2 },
    },
    {
        {  0, -6 }, { 20, 13 }, { 12, 13 }, {  6, 13 }, {  5, 13 }, { 21, 12 },
        { 19, 12 }, { 18, 10 }, {  4,  9 }, { 11,  8 }, { 17,  7 }, { 16,  6 },
        {  3,  6 }, { 15,  5 }, { 10,  5 }, {  2,  5 }, {  0,  5 }, {  9,  4 },
        {  8,  4 }, {  1,  4 }, {  7,  3 }, { 14,  2 }, { 13,  2 },
    },
    {
        {  0, -6 }, {  0,-15 }, { 12, 15 }, {  6, 14 }, { 21, 13 }, { 20, 13 },
        {  5, 13 }, { 19, 11 }, { 11, 10 }, {  4,  9 }, { 18,  8 }, { 10,  7 },
        {  3,  7 }, {  0,  7 }, { 17,  6 }, { 16,  6 }, {  9,  6 }, {  2,  5 },
        {  8,  4 }, {  1,  4 }, { 15,  3 }, {  7,  3 }, { 14,  2 }, { 13,  2 },
    },
    {
        {  0, -6 }, {  0,-15 }, { 20, 15 }, { 12, 14 }, { 11, 13 }, {  6, 13 },
        {  5, 13 }, { 21, 12 }, {  4, 12 }, { 19, 11 }, { 10, 11 }, {  3, 10 },
        {  0, 10 }, {  9,  8 }, { 18,  7 }, {  8,  7 }, {  2,  7 }, { 17,  6 },
        {  7,  5 }, {  1,  5 }, { 16,  3 }, { 15,  2 }, { 14,  2 }, { 13,  2 },
    },
    {
        {  0, -6 }, {  0,-15 }, { 12, 15 }, { 11, 14 }, {  6, 14 }, {  5, 14 },
        { 21, 12 }, { 20, 12 }, { 10, 12 }, {  4, 11 }, {  0, 11 }, {  9, 10 },
        {  3, 10 }, { 19,  8 }, {  8,  8 }, {  2,  8 }, { 18,  6 }, {  7,  6 },
        {  1,  4 }, { 17,  3 }, { 14,  3 }, { 13,  3 }, { 16,  2 }, { 15,  2 },
    },
    {
        {  0, -6 }, { 12, 12 }, {  6, 12 }, { 21, 11 }, { 11, 11 }, {  5, 11 },
        { 20, 10 }, { 10, 10 }, {  9,  9 }, {  0,  9 }, {  8,  8 }, {  2,  8 },
        { 19,  7 }, {  7,  7 }, {  4,  7 }, {  3,  7 }, { 18,  5 }, {  1,  5 },
        { 14,  4 }, { 13,  4 }, { 17,  2 }, { 16,  2 }, { 15,  2 },
    },
    {
        {  0, -6 }, { 12, 13 }, {  6, 13 }, { 21, 12 }, { 11, 12 }, {  5, 12 },
        { 20, 11 }, {  3, 11 }, { 10, 10 }, {  9, 10 }, {  2, 10 }, {  0, 10 },
        {  8,  9 }, {  7,  8 }, {  4,  8 }, { 19,  6 }, {  1,  6 }, { 13,  4 },
        { 18,  3 }, { 15,  3 }, { 14,  3 }, { 17,  2 }, { 16,  2 },
    },
    {
        {  0, -6 }, { 12, 12 }, { 11, 12 }, {  6, 12 }, {  0, 12 }, { 21, 10 },
        { 10, 10 }, {  5, 10 }, { 20,  8 }, {  9,  8 }, {  2,  8 }, {  8,  7 },
        {  7,  7 }, {  4,  6 }, {  3,  6 }, {  1,  6 }, { 13,  5 }, { 19,  4 },
        { 14,  4 }, { 16,  3 }, { 15,  3 }, { 18,  2 }, { 17,  2 },
    },
    {
        {  0, -6 }, { 12, 13 }, {  6, 13 }, {  5, 13 }, {  0, 13 }, {  4, 11 },
        { 11, 10 }, { 21,  9 }, { 10,  9 }, {  9,  9 }, {  8,  8 }, {  2,  8 },
        {  7,  7 }, {  1,  7 }, { 20,  6 }, { 14,  5 }, { 13,  5 }, { 15,  4 },
        {  3,  4 }, { 17,  3 }, { 16,  3 }, { 19,  2 }, { 18,  2 },
    },
    {
        {  0, -6 }, {  6, 11 }, {  5, 11 }, { 12, 10 }, { 11, 10 }, {  0, 10 },
        { 21,  9 }, { 10,  9 }, {  4,  9 }, {  3,  9 }, {  9,  8 }, {  8,  6 },
        {  2,  6 }, {  7,  5 }, {  1,  5 }, { 18,  4 }, { 17,  4 }, { 16,  4 },
        { 15,  4 }, { 19,  3 }, { 14,  3 }, { 13,  3 }, { 20,  2 },
    },
};

#endif /* AVCODEC_MPEG4DATA_H */
