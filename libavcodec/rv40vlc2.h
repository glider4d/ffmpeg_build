/*
 * RealVideo 4 decoder
 * copyright (c) 2007 Konstantin Shishkov
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
 * RV40 VLC tables used for macroblock information decoding
 */

#ifndef AVCODEC_RV40VLC2_H
#define AVCODEC_RV40VLC2_H

#include <stdint.h>

/**
 * codes used for the first four block types
 */
//@{
#define AIC_TOP_BITS  7
#define AIC_TOP_SIZE 16
static const uint8_t rv40_aic_top_vlc_tab[AIC_TOP_SIZE][2] = {
    { 0x3, 5 }, { 0x2, 5 }, { 0x7, 5 }, { 0x4, 5 }, { 0xF, 3 }, { 0x8, 4 },
    { 0x1, 4 }, { 0xE, 5 }, { 0xC, 5 }, { 0xB, 6 }, { 0x6, 6 }, { 0x9, 7 },
    { 0x5, 7 }, { 0xD, 7 }, { 0xA, 7 }, { 0x0, 1 },
};
//@}

/**
 * codes used for determining a pair of block types
 */
//@{
#define AIC_MODE2_NUM  20
#define AIC_MODE2_SIZE 81
#define AIC_MODE2_BITS  9

static const uint8_t aic_mode2_vlc_syms[AIC_MODE2_NUM][AIC_MODE2_SIZE] = {
    {
        0x06, 0x01, 0x28, 0x11, 0x22, 0x20, 0x04, 0x07, 0x05, 0x16, 0x08, 0x60,
        0x02, 0x66, 0x61, 0x23, 0x15, 0x31, 0x26, 0x77, 0x12, 0x10, 0x42, 0x21,
        0x72, 0x50, 0x88, 0x82, 0x27, 0x24, 0x44, 0x40, 0x62, 0x55, 0x18, 0x13,
        0x53, 0x52, 0x64, 0x46, 0x41, 0x51, 0x47, 0x33, 0x32, 0x74, 0x67, 0x14,
        0x03, 0x30, 0x25, 0x36, 0x34, 0x45, 0x37, 0x78, 0x85, 0x84, 0x75, 0x73,
        0x57, 0x86, 0x65, 0x58, 0x71, 0x68, 0x70, 0x56, 0x54, 0x17, 0x76, 0x63,
        0x87, 0x81, 0x80, 0x38, 0x35, 0x48, 0x43, 0x83, 0x00,
    },
    {
        0x77, 0x62, 0x46, 0x33, 0x68, 0x52, 0x18, 0x05, 0x31, 0x25, 0x42, 0x04,
        0x51, 0x44, 0x40, 0x32, 0x55, 0x41, 0x26, 0x24, 0x30, 0x27, 0x57, 0x36,
        0x84, 0x76, 0x81, 0x83, 0x78, 0x73, 0x45, 0x85, 0x75, 0x86, 0x87, 0x56,
        0x53, 0x80, 0x67, 0x35, 0x34, 0x43, 0x37, 0x82, 0x70, 0x65, 0x74, 0x71,
        0x54, 0x48, 0x63, 0x58, 0x72, 0x64, 0x07, 0x03, 0x47, 0x38, 0x88, 0x20,
        0x01, 0x60, 0x28, 0x16, 0x02, 0x61, 0x50, 0x13, 0x08, 0x23, 0x17, 0x66,
        0x14, 0x06, 0x21, 0x15, 0x10, 0x00, 0x22, 0x12, 0x11,
    },
    {
        0x10, 0x01, 0x24, 0x12, 0x88, 0x83, 0x70, 0x14, 0x08, 0x46, 0x25, 0x87,
        0x44, 0x42, 0x60, 0x50, 0x61, 0x51, 0x71, 0x66, 0x15, 0x04, 0x41, 0x33,
        0x17, 0x03, 0x34, 0x18, 0x86, 0x68, 0x67, 0x63, 0x57, 0x75, 0x65, 0x45,
        0x36, 0x58, 0x54, 0x43, 0x37, 0x56, 0x53, 0x74, 0x62, 0x85, 0x78, 0x07,
        0x05, 0x35, 0x13, 0x84, 0x64, 0x55, 0x76, 0x73, 0x47, 0x38, 0x52, 0x48,
        0x11, 0x00, 0x21, 0x20, 0x82, 0x28, 0x81, 0x80, 0x31, 0x30, 0x40, 0x32,
        0x16, 0x06, 0x26, 0x23, 0x27, 0x02, 0x77, 0x72, 0x22,
    },
    {
        0x88, 0x82, 0x50, 0x47, 0x74, 0x43, 0x76, 0x75, 0x68, 0x24, 0x00, 0x37,
        0x30, 0x54, 0x44, 0x64, 0x58, 0x36, 0x34, 0x65, 0x40, 0x06, 0x03, 0x20,
        0x17, 0x08, 0x04, 0x48, 0x45, 0x87, 0x84, 0x71, 0x85, 0x78, 0x80, 0x73,
        0x07, 0x86, 0x63, 0x57, 0x70, 0x67, 0x77, 0x72, 0x83, 0x81, 0x32, 0x28,
        0x18, 0x05, 0x42, 0x38, 0x52, 0x14, 0x02, 0x25, 0x23, 0x66, 0x56, 0x53,
        0x62, 0x60, 0x35, 0x33, 0x46, 0x41, 0x21, 0x12, 0x10, 0x01, 0x26, 0x13,
        0x61, 0x22, 0x16, 0x27, 0x15, 0x55, 0x31, 0x51, 0x11,
    },
    {
        0x13, 0x10, 0x27, 0x14, 0x66, 0x44, 0x41, 0x53, 0x46, 0x25, 0x18, 0x35,
        0x33, 0x68, 0x63, 0x88, 0x71, 0x34, 0x24, 0x50, 0x47, 0x37, 0x08, 0x48,
        0x45, 0x82, 0x74, 0x70, 0x84, 0x76, 0x30, 0x04, 0x57, 0x43, 0x83, 0x78,
        0x86, 0x85, 0x73, 0x07, 0x80, 0x75, 0x87, 0x65, 0x56, 0x03, 0x00, 0x20,
        0x06, 0x72, 0x52, 0x31, 0x62, 0x60, 0x21, 0x15, 0x51, 0x22, 0x16, 0x12,
        0x02, 0x01, 0x23, 0x17, 0x55, 0x77, 0x64, 0x58, 0x54, 0x81, 0x67, 0x36,
        0x05, 0x40, 0x38, 0x28, 0x26, 0x42, 0x32, 0x61, 0x11,
    },
    {
        0x22, 0x11, 0x23, 0x13, 0x32, 0x25, 0x72, 0x45, 0x40, 0x57, 0x50, 0x03,
        0x00, 0x30, 0x08, 0x86, 0x75, 0x80, 0x68, 0x43, 0x04, 0x47, 0x07, 0x48,
        0x65, 0x58, 0x85, 0x84, 0x78, 0x64, 0x67, 0x63, 0x74, 0x73, 0x06, 0x05,
        0x44, 0x37, 0x87, 0x83, 0x46, 0x42, 0x54, 0x53, 0x18, 0x17, 0x36, 0x34,
        0x66, 0x55, 0x38, 0x24, 0x56, 0x41, 0x02, 0x01, 0x20, 0x14, 0x16, 0x12,
        0x61, 0x27, 0x26, 0x15, 0x31, 0x28, 0x21, 0x88, 0x82, 0x62, 0x60, 0x76,
        0x70, 0x33, 0x10, 0x52, 0x35, 0x71, 0x51, 0x81, 0x77,
    },
    {
        0x16, 0x12, 0x61, 0x22, 0x51, 0x21, 0x15, 0x52, 0x31, 0x55, 0x81, 0x66,
        0x46, 0x42, 0x62, 0x54, 0x14, 0x01, 0x25, 0x23, 0x27, 0x13, 0x56, 0x41,
        0x35, 0x33, 0x58, 0x53, 0x26, 0x10, 0x32, 0x28, 0x84, 0x77, 0x43, 0x00,
        0x68, 0x47, 0x85, 0x45, 0x34, 0x57, 0x50, 0x05, 0x03, 0x37, 0x30, 0x87,
        0x86, 0x76, 0x08, 0x07, 0x80, 0x75, 0x70, 0x73, 0x04, 0x78, 0x74, 0x63,
        0x48, 0x71, 0x67, 0x82, 0x65, 0x17, 0x06, 0x24, 0x20, 0x88, 0x83, 0x60,
        0x44, 0x40, 0x72, 0x64, 0x18, 0x02, 0x38, 0x36, 0x11,
    },
    {
        0x66, 0x01, 0x14, 0x03, 0x24, 0x23, 0x62, 0x08, 0x04, 0x35, 0x20, 0x72,
        0x63, 0x58, 0x70, 0x68, 0x48, 0x45, 0x54, 0x53, 0x26, 0x25, 0x65, 0x44,
        0x13, 0x05, 0x51, 0x41, 0x61, 0x16, 0x12, 0x10, 0x06, 0x28, 0x27, 0x60,
        0x40, 0x32, 0x56, 0x52, 0x17, 0x00, 0x31, 0x18, 0x73, 0x30, 0x75, 0x74,
        0x88, 0x84, 0x83, 0x86, 0x80, 0x85, 0x43, 0x38, 0x87, 0x47, 0x76, 0x71,
        0x50, 0x37, 0x78, 0x57, 0x82, 0x77, 0x67, 0x33, 0x07, 0x36, 0x34, 0x81,
        0x46, 0x42, 0x64, 0x55, 0x15, 0x02, 0x22, 0x21, 0x11,
    },
    {
        0x22, 0x00, 0x11, 0x01, 0x20, 0x08, 0x88, 0x62, 0x27, 0x02, 0x07, 0x06,
        0x21, 0x12, 0x28, 0x87, 0x66, 0x32, 0x30, 0x38, 0x33, 0x15, 0x14, 0x25,
        0x17, 0x26, 0x24, 0x60, 0x42, 0x78, 0x71, 0x84, 0x81, 0x44, 0x23, 0x52,
        0x50, 0x80, 0x70, 0x35, 0x13, 0x48, 0x46, 0x83, 0x51, 0x40, 0x64, 0x61,
        0x55, 0x47, 0x74, 0x58, 0x18, 0x03, 0x41, 0x31, 0x73, 0x68, 0x43, 0x36,
        0x57, 0x45, 0x85, 0x56, 0x54, 0x67, 0x65, 0x76, 0x75, 0x37, 0x34, 0x63,
        0x53, 0x86, 0x77, 0x72, 0x05, 0x04, 0x16, 0x10, 0x82,
    },
    {
        0x22, 0x12, 0x27, 0x21, 0x14, 0x01, 0x20, 0x15, 0x62, 0x02, 0x00, 0x16,
        0x10, 0x60, 0x17, 0x72, 0x66, 0x28, 0x07, 0x06, 0x18, 0x13, 0x82, 0x55,
        0x40, 0x78, 0x71, 0x25, 0x23, 0x32, 0x31, 0x42, 0x26, 0x61, 0x52, 0x24,
        0x08, 0x50, 0x44, 0x77, 0x37, 0x34, 0x63, 0x57, 0x87, 0x84, 0x80, 0x86,
        0x83, 0x45, 0x36, 0x85, 0x73, 0x70, 0x68, 0x81, 0x74, 0x33, 0x03, 0x38,
        0x35, 0x88, 0x56, 0x54, 0x76, 0x75, 0x46, 0x43, 0x53, 0x48, 0x64, 0x51,
        0x58, 0x47, 0x67, 0x65, 0x05, 0x04, 0x41, 0x30, 0x11,
    },
    {
        0x44, 0x40, 0x66, 0x60, 0x23, 0x16, 0x32, 0x30, 0x07, 0x06, 0x18, 0x14,
        0x86, 0x50, 0x47, 0x75, 0x55, 0x33, 0x31, 0x41, 0x35, 0x80, 0x70, 0x84,
        0x83, 0x78, 0x71, 0x87, 0x81, 0x77, 0x12, 0x38, 0x25, 0x52, 0x51, 0x08,
        0x01, 0x17, 0x15, 0x58, 0x54, 0x68, 0x67, 0x34, 0x13, 0x48, 0x46, 0x04,
        0x03, 0x36, 0x05, 0x85, 0x56, 0x45, 0x65, 0x57, 0x73, 0x43, 0x37, 0x63,
        0x53, 0x64, 0x61, 0x76, 0x74, 0x24, 0x10, 0x62, 0x42, 0x21, 0x11, 0x02,
        0x00, 0x26, 0x20, 0x88, 0x28, 0x27, 0x82, 0x72, 0x22,
    },
    {
        0x28, 0x27, 0x71, 0x70, 0x87, 0x78, 0x82, 0x22, 0x77, 0x72, 0x07, 0x01,
        0x14, 0x08, 0x81, 0x66, 0x62, 0x80, 0x75, 0x40, 0x25, 0x51, 0x50, 0x02,
        0x00, 0x26, 0x20, 0x42, 0x24, 0x52, 0x44, 0x16, 0x10, 0x23, 0x17, 0x64,
        0x60, 0x86, 0x83, 0x46, 0x41, 0x55, 0x54, 0x48, 0x47, 0x58, 0x56, 0x18,
        0x04, 0x35, 0x31, 0x57, 0x38, 0x34, 0x03, 0x63, 0x53, 0x67, 0x13, 0x05,
        0x36, 0x33, 0x73, 0x61, 0x43, 0x37, 0x68, 0x65, 0x45, 0x85, 0x76, 0x74,
        0x15, 0x06, 0x32, 0x30, 0x84, 0x12, 0x11, 0x88, 0x21,
    },
    {
        0x82, 0x27, 0x08, 0x07, 0x16, 0x10, 0x62, 0x76, 0x74, 0x50, 0x41, 0x86,
        0x67, 0x15, 0x14, 0x38, 0x17, 0x40, 0x23, 0x61, 0x42, 0x11, 0x02, 0x26,
        0x20, 0x72, 0x77, 0x28, 0x12, 0x01, 0x60, 0x24, 0x88, 0x52, 0x44, 0x75,
        0x64, 0x25, 0x06, 0x32, 0x30, 0x55, 0x51, 0x68, 0x58, 0x46, 0x33, 0x48,
        0x47, 0x43, 0x37, 0x56, 0x54, 0x13, 0x03, 0x35, 0x34, 0x85, 0x73, 0x65,
        0x63, 0x45, 0x36, 0x57, 0x53, 0x83, 0x80, 0x05, 0x04, 0x31, 0x18, 0x84,
        0x70, 0x66, 0x87, 0x81, 0x21, 0x00, 0x78, 0x71, 0x22,
    },
    {
        0x84, 0x81, 0x32, 0x25, 0x86, 0x83, 0x10, 0x04, 0x23, 0x14, 0x62, 0x26,
        0x70, 0x66, 0x42, 0x24, 0x64, 0x44, 0x87, 0x75, 0x71, 0x85, 0x80, 0x55,
        0x52, 0x67, 0x61, 0x50, 0x48, 0x58, 0x51, 0x18, 0x15, 0x38, 0x30, 0x33,
        0x31, 0x65, 0x41, 0x05, 0x03, 0x13, 0x08, 0x37, 0x36, 0x53, 0x43, 0x60,
        0x73, 0x68, 0x63, 0x57, 0x45, 0x35, 0x56, 0x54, 0x78, 0x76, 0x40, 0x34,
        0x47, 0x46, 0x06, 0x01, 0x17, 0x07, 0x77, 0x28, 0x02, 0x00, 0x12, 0x11,
        0x88, 0x72, 0x27, 0x20, 0x16, 0x74, 0x21, 0x82, 0x22,
    },
    {
        0x77, 0x12, 0x81, 0x78, 0x72, 0x38, 0x33, 0x50, 0x44, 0x23, 0x01, 0x31,
        0x25, 0x34, 0x13, 0x37, 0x35, 0x86, 0x58, 0x55, 0x73, 0x68, 0x43, 0x04,
        0x54, 0x53, 0x64, 0x46, 0x65, 0x63, 0x57, 0x45, 0x67, 0x05, 0x03, 0x56,
        0x36, 0x62, 0x52, 0x83, 0x66, 0x42, 0x32, 0x80, 0x71, 0x10, 0x08, 0x24,
        0x16, 0x87, 0x18, 0x17, 0x47, 0x41, 0x07, 0x06, 0x15, 0x14, 0x75, 0x74,
        0x85, 0x76, 0x51, 0x48, 0x61, 0x60, 0x40, 0x30, 0x84, 0x70, 0x22, 0x82,
        0x28, 0x21, 0x11, 0x02, 0x00, 0x26, 0x20, 0x27, 0x88,
    },
    {
        0x72, 0x27, 0x87, 0x81, 0x42, 0x33, 0x52, 0x50, 0x16, 0x08, 0x31, 0x23,
        0x02, 0x00, 0x78, 0x20, 0x82, 0x88, 0x28, 0x38, 0x32, 0x71, 0x62, 0x24,
        0x10, 0x30, 0x26, 0x64, 0x61, 0x76, 0x75, 0x44, 0x18, 0x58, 0x55, 0x54,
        0x48, 0x45, 0x43, 0x57, 0x46, 0x73, 0x35, 0x34, 0x37, 0x36, 0x04, 0x03,
        0x13, 0x05, 0x85, 0x65, 0x63, 0x68, 0x67, 0x47, 0x41, 0x56, 0x53, 0x70,
        0x60, 0x07, 0x06, 0x15, 0x14, 0x84, 0x83, 0x80, 0x66, 0x51, 0x86, 0x74,
        0x17, 0x01, 0x40, 0x25, 0x12, 0x11, 0x77, 0x21, 0x22,
    },
    {
        0x27, 0x21, 0x88, 0x72, 0x28, 0x12, 0x11, 0x83, 0x77, 0x82, 0x80, 0x78,
        0x25, 0x24, 0x38, 0x32, 0x08, 0x01, 0x18, 0x16, 0x20, 0x02, 0x26, 0x23,
        0x81, 0x74, 0x86, 0x84, 0x10, 0x00, 0x62, 0x61, 0x04, 0x03, 0x13, 0x06,
        0x76, 0x58, 0x51, 0x64, 0x60, 0x31, 0x17, 0x41, 0x35, 0x44, 0x42, 0x70,
        0x66, 0x33, 0x15, 0x48, 0x40, 0x87, 0x65, 0x55, 0x67, 0x63, 0x54, 0x07,
        0x57, 0x56, 0x46, 0x45, 0x50, 0x47, 0x73, 0x68, 0x30, 0x14, 0x53, 0x43,
        0x75, 0x71, 0x52, 0x34, 0x05, 0x37, 0x36, 0x85, 0x22,
    },
    {
        0x21, 0x14, 0x32, 0x27, 0x10, 0x72, 0x67, 0x82, 0x77, 0x44, 0x40, 0x54,
        0x45, 0x41, 0x37, 0x68, 0x56, 0x08, 0x01, 0x24, 0x13, 0x78, 0x48, 0x04,
        0x03, 0x43, 0x05, 0x81, 0x25, 0x07, 0x47, 0x35, 0x34, 0x06, 0x46, 0x36,
        0x88, 0x84, 0x83, 0x75, 0x74, 0x76, 0x73, 0x86, 0x85, 0x70, 0x65, 0x80,
        0x71, 0x57, 0x53, 0x87, 0x63, 0x51, 0x50, 0x23, 0x18, 0x33, 0x31, 0x61,
        0x12, 0x62, 0x22, 0x02, 0x00, 0x17, 0x15, 0x66, 0x55, 0x42, 0x64, 0x58,
        0x26, 0x20, 0x38, 0x30, 0x28, 0x16, 0x60, 0x52, 0x11,
    },
    {
        0x61, 0x15, 0x27, 0x25, 0x32, 0x28, 0x06, 0x01, 0x24, 0x17, 0x78, 0x75,
        0x87, 0x80, 0x85, 0x08, 0x07, 0x73, 0x70, 0x48, 0x47, 0x84, 0x83, 0x81,
        0x71, 0x88, 0x86, 0x76, 0x74, 0x30, 0x04, 0x43, 0x37, 0x82, 0x72, 0x67,
        0x20, 0x03, 0x57, 0x45, 0x77, 0x52, 0x36, 0x41, 0x38, 0x68, 0x58, 0x05,
        0x00, 0x34, 0x33, 0x55, 0x26, 0x60, 0x56, 0x11, 0x66, 0x16, 0x64, 0x51,
        0x10, 0x02, 0x23, 0x18, 0x62, 0x54, 0x53, 0x65, 0x63, 0x40, 0x35, 0x50,
        0x42, 0x31, 0x13, 0x46, 0x44, 0x14, 0x12, 0x22, 0x21,
    },
    {
        0x22, 0x21, 0x12, 0x01, 0x16, 0x15, 0x82, 0x88, 0x87, 0x42, 0x40, 0x60,
        0x44, 0x06, 0x05, 0x31, 0x10, 0x66, 0x61, 0x83, 0x76, 0x26, 0x25, 0x72,
        0x27, 0x81, 0x11, 0x46, 0x41, 0x84, 0x62, 0x13, 0x02, 0x24, 0x20, 0x80,
        0x75, 0x65, 0x63, 0x68, 0x54, 0x37, 0x07, 0x57, 0x30, 0x50, 0x52, 0x48,
        0x73, 0x67, 0x74, 0x55, 0x53, 0x47, 0x58, 0x56, 0x34, 0x04, 0x43, 0x38,
        0x70, 0x64, 0x17, 0x00, 0x45, 0x36, 0x78, 0x86, 0x85, 0x33, 0x32, 0x51,
        0x35, 0x08, 0x03, 0x18, 0x14, 0x28, 0x23, 0x77, 0x71,
    },
};

static const uint8_t aic_mode2_vlc_bits[AIC_MODE2_NUM][AIC_MODE2_SIZE] = {
    {
         5,  5,  5,  5,  3,  6,  6,  8,  8,  8,  8,  6,  4,  7,  7,  9,  9,
         9,  9,  7,  7,  7,  7,  7,  8,  8,  8,  8,  8,  8,  8,  8,  9,  9,
        11, 11, 11, 11,  9,  9,  9,  9,  9, 10, 10, 10, 10, 10, 10, 10, 10,
        14, 14, 14, 14, 12, 13, 13, 15, 15, 14, 13, 12, 12, 12, 12, 11, 11,
        13, 13, 13, 13, 11, 10, 10, 12, 12, 12, 12, 10,  1,
    },
    {
         8,  8, 10, 10, 10, 10, 10, 10, 10, 10,  8,  8,  8,  8,  9,  9,  9,
         9,  9,  9,  9,  9, 13, 13, 13, 13, 11, 12, 12, 14, 14, 15, 15, 14,
        12, 11, 11, 11, 11, 12, 12, 12, 12, 10, 12, 12, 12, 12, 12, 12, 12,
        12,  9,  9, 11, 11, 11, 11,  9,  6,  6,  6,  6,  5,  5,  7,  7,  9,
         9,  9,  9,  7,  7,  7,  7,  7,  4,  4,  4,  4,  1,
    },
    {
         7,  7,  7,  7,  5,  8,  8, 10, 10, 10, 10,  8,  8,  8,  8,  8,  9,
         9,  9,  9,  9,  9,  9,  9, 12, 12, 12, 12, 10, 13, 13, 14, 14, 14,
        14, 13, 13, 13, 13, 12, 12, 12, 12, 10, 10, 10, 10, 11, 11, 11, 11,
         9, 11, 11, 11, 11, 11, 11, 11, 11,  5,  5,  5,  5,  4,  4,  6,  6,
         8,  8,  8,  8,  8,  8,  8,  8,  6,  6,  6,  6,  1,
    },
    {
         9,  9, 11, 11, 13, 13, 13, 13, 11, 11, 11, 11, 11,  9,  9,  9,  9,
        10, 10, 10, 10, 10, 10, 10, 10, 12, 12, 12, 12, 10, 12, 12, 13, 13,
        15, 15, 14, 13, 12, 12, 12, 12, 10, 10, 10, 10,  7,  7,  9,  9,  9,
         9,  7,  8,  8,  8,  8,  6,  8,  8,  8,  8,  8,  8,  8,  8,  5,  5,
         7,  7,  7,  7,  5,  4,  4,  6,  6,  6,  6,  4,  1,
    },
    {
         7,  7,  7,  7,  5,  9,  9,  9,  9,  9,  9,  9,  9, 11, 11, 11, 11,
        11, 11, 11, 11, 12, 12, 12, 12, 10, 13, 13, 13, 13, 13, 13, 13, 13,
        12, 12, 14, 14, 15, 15, 15, 15, 12,  9,  9, 11, 11, 11, 11,  9,  7,
         7,  7,  7,  5,  5,  5,  5,  4,  4,  8,  8,  8,  8,  6,  8,  8, 10,
        10, 10, 10, 10, 10, 10, 10,  8,  8,  8,  8,  4,  1,
    },
    {
         2,  2,  7,  7,  7,  7,  5, 12, 12, 12, 12, 12, 12, 12, 12, 11, 11,
        12, 12, 13, 13, 14, 14, 13, 10, 10, 10, 10,  9,  9, 11, 11, 11, 11,
        11, 11, 11, 11,  8,  8, 10, 10, 10, 10, 10, 10, 10, 10,  7,  7,  9,
         9,  9,  9,  9,  9,  9,  9,  5,  5,  5,  5,  6,  6,  6,  6,  4,  6,
         6,  8,  8,  8,  8,  8,  8,  8,  8,  6,  6,  6,  6,
    },
    {
         5,  5,  5,  5,  3,  6,  6,  6,  6,  4,  7,  7,  9,  9,  9,  9,  9,
         9,  9,  9,  7,  7,  7,  7,  8,  8,  8,  8,  8,  8,  8,  8, 11, 11,
        13, 13, 13, 13, 11, 11, 11, 11, 11, 12, 12, 12, 12, 10, 12, 12, 15,
        15, 15, 15, 13, 14, 14, 14, 14, 12, 12, 12, 12,  9,  9, 11, 11, 11,
        11,  9,  8,  8, 10, 10, 10, 10, 10, 10, 10, 10,  1,
    },
    {
         5,  5,  9,  9,  9,  9,  7, 11, 11, 11, 11,  9, 11, 11, 11, 11, 11,
        11, 11, 11,  9,  9,  9,  9,  7,  7,  7,  7,  3,  4,  4,  8,  8,  8,
         8,  6, 10, 10, 10, 10, 10, 10, 10, 10, 14, 14, 14, 14, 12, 13, 13,
        14, 14, 13, 12, 12, 12, 12, 11, 11, 13, 13, 13, 13, 11, 10, 10, 12,
        12, 12, 12, 10,  8,  8,  8,  8,  6,  6,  6,  6,  1,
    },
    {
         2,  2,  5,  5,  7,  7,  7,  7,  5,  3,  6,  6,  6,  6,  4,  8,  8,
        10, 10, 10, 10, 10, 10, 10, 10,  8,  8,  8,  8,  9,  9,  9,  9,  9,
         9,  9,  9, 10, 10, 12, 12, 12, 12, 10, 10, 10, 10, 10, 11, 11, 11,
        11, 11, 11, 11, 11, 12, 12, 14, 14, 14, 14, 12, 12, 12, 12, 12, 11,
        11, 13, 13, 13, 13, 11,  6,  6,  8,  8,  8,  8,  6,
    },
    {
         3,  3,  6,  6,  8,  8,  8,  8,  6,  6,  6,  6,  6,  7,  7,  7,  7,
         5, 10, 10, 10, 10,  8, 10, 10, 10, 10, 10, 10, 10, 10,  8,  8,  8,
         8,  9,  9,  9,  9,  7, 13, 13, 13, 13, 11, 12, 12, 13, 13, 14, 14,
        14, 14, 11, 11, 11, 11, 12, 12, 12, 12, 10, 12, 12, 12, 12, 12, 12,
        12, 12,  9,  9, 11, 11, 11, 11, 11, 11, 11, 11,  1,
    },
    {
         9,  9,  9,  9,  9,  9,  9,  9, 11, 11, 11, 11,  9, 11, 11, 11, 11,
        11, 11, 11, 11,  9,  9,  9,  9,  7,  7,  7,  7,  6,  6, 10, 10, 10,
        10, 10, 10, 10, 10, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13,
        11, 14, 14, 14, 14, 12, 13, 13, 13, 13, 10, 10, 10, 10,  8,  8,  8,
         8,  5,  5,  7,  7,  7,  7,  5,  4,  4,  4,  4,  1,
    },
    {
         4,  4,  6,  6,  6,  6,  4,  2,  3,  3,  9,  9,  9,  9,  7,  9,  9,
         9,  9,  9,  9,  9,  9,  7,  7,  7,  7,  8,  8,  8,  8,  8,  8,  8,
         8, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11,
        12, 12, 14, 14, 14, 14, 12, 12, 12, 12, 12, 11, 11, 13, 13, 14, 14,
        13, 11,  8,  8, 10, 10, 10, 10,  8,  5,  5,  5,  5,
    },
    {
         5,  5,  9,  9,  9,  9,  7,  9,  9, 11, 11, 11, 11, 11, 11, 11, 11,
         9,  9,  9,  9,  7,  7,  7,  7,  3,  4,  4,  8,  8,  8,  8,  6, 10,
        10, 10, 10, 10, 10, 10, 10, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13,
        13, 13, 13, 13, 13, 13, 12, 12, 13, 13, 14, 14, 14, 14, 10, 10, 12,
        12, 12, 12, 10,  8,  8,  8,  8,  6,  6,  6,  6,  1,
    },
    {
         7,  7,  9,  9,  9,  9,  9,  9,  9,  9,  7,  7,  7,  7,  8,  8,  8,
         8,  6, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11,
        11, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 11, 12, 12, 13,
        13, 14, 14, 14, 14,  8,  8, 10, 10, 10, 10, 10, 10, 10, 10,  5,  5,
         7,  7,  7,  7,  5,  4,  4,  6,  6,  6,  6,  4,  1,
    },
    {
         6,  6,  6,  6,  4,  9,  9,  9,  9,  9,  9,  9,  9, 11, 11, 11, 11,
         9, 12, 12, 12, 12, 12, 12, 12, 12, 11, 11, 13, 13, 14, 14, 13, 13,
        13, 13, 13,  9,  9,  9,  9,  7,  7,  7,  7,  8,  8,  8,  8,  6, 10,
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  8,  8,
         8,  8,  2,  3,  3,  5,  5,  7,  7,  7,  7,  5,  3,
    },
    {
         5,  5,  7,  7,  9,  9,  9,  9,  9,  9,  9,  9,  7,  7,  7,  7,  3,
         4,  4,  8,  8,  8,  8,  8,  8,  8,  8, 11, 11, 11, 11, 11, 11, 11,
        11, 12, 12, 14, 14, 14, 14, 12, 12, 12, 12, 12, 13, 13, 13, 13, 11,
        13, 13, 13, 13, 13, 13, 13, 13,  9,  9, 11, 11, 11, 11,  9,  8,  8,
        10, 10, 10, 10, 10, 10, 10, 10,  6,  6,  6,  6,  1,
    },
    {
         5,  5,  5,  5,  3,  6,  6,  6,  6,  4,  7,  7,  9,  9,  9,  9,  9,
         9,  9,  9,  7,  7,  7,  7,  8,  8,  8,  8,  8,  8,  8,  8, 11, 11,
        11, 11,  9, 11, 11, 11, 11, 11, 11, 11, 11,  9,  9,  9,  9, 10, 10,
        10, 10,  8, 12, 12, 13, 13, 14, 14, 14, 14, 12, 12, 12, 12, 11, 11,
        13, 13, 13, 13, 11, 10, 10, 12, 12, 12, 12, 10,  1,
    },
    {
         7,  7,  7,  7,  5,  9,  9,  9,  9,  9,  9,  9,  9, 10, 10, 10, 10,
        10, 10, 10, 10, 11, 11, 13, 13, 13, 13, 11, 11, 11, 11, 11, 12, 12,
        12, 12, 10, 13, 13, 14, 14, 15, 15, 15, 15, 13, 13, 13, 13, 12, 12,
        12, 12,  7,  7,  9,  9,  9,  9,  7,  3,  4,  4,  8,  8,  8,  8,  6,
         8,  8,  8,  8,  8,  8,  8,  8,  6,  6,  6,  6,  1,
    },
    {
         4,  4,  8,  8,  8,  8,  8,  8,  8,  8, 13, 13, 14, 14, 13, 13, 13,
        13, 13, 12, 12, 12, 12, 11, 11, 11, 11, 10, 10, 12, 12, 12, 12, 10,
         9,  9, 11, 11, 11, 11,  9,  8,  8, 10, 10, 10, 10, 10, 10, 10, 10,
         6,  6,  6,  6,  2,  3,  3,  5,  5,  9,  9,  9,  9,  7,  9,  9,  9,
         9,  9,  9,  9,  9,  7,  7,  7,  7,  5,  5,  5,  5,
    },
    {
         2,  2,  7,  7,  7,  7,  5,  7,  7,  9,  9,  9,  9,  9,  9,  9,  9,
         7,  7,  7,  7,  5,  5,  5,  5,  4,  4,  8,  8,  8,  8,  8,  8,  8,
         8, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 14, 11, 11, 11, 11, 10,
        10, 12, 12, 12, 12, 12, 12, 12, 12,  9,  9, 11, 11, 11, 11,  9,  8,
         8, 10, 10, 10, 10, 10, 10, 10, 10,  6,  6,  6,  6,
    },
};
//@}

/**
 * Codes used for determining block type
 */
//@{
#define AIC_MODE1_NUM  90
#define AIC_MODE1_SIZE  9
#define AIC_MODE1_BITS  7

static const uint8_t aic_mode1_vlc_tabs[AIC_MODE1_NUM][AIC_MODE1_SIZE][2] = {
    { { 4, 4 }, { 1, 4 }, { 7, 7 }, { 3, 7 }, { 5, 6 },
      { 8, 5 }, { 6, 4 }, { 2, 2 }, { 0, 1 } },
    { { 5, 3 }, { 2, 3 }, { 8, 7 }, { 7, 7 }, { 4, 6 },
      { 0, 5 }, { 3, 4 }, { 6, 3 }, { 1, 1 } },
    { { 7, 4 }, { 1, 4 }, { 5, 7 }, { 3, 7 }, { 4, 6 },
      { 6, 5 }, { 8, 4 }, { 2, 2 }, { 0, 1 } },
    { { 2, 3 }, { 1, 3 }, { 7, 7 }, { 4, 7 }, { 8, 6 },
      { 6, 5 }, { 5, 4 }, { 3, 3 }, { 0, 1 } },
    { { 2, 2 }, { 0, 2 }, { 4, 4 }, { 1, 4 }, { 8, 6 },
      { 3, 6 }, { 5, 5 }, { 7, 4 }, { 6, 2 } },
    { { 4, 7 }, { 0, 7 }, { 8, 7 }, { 7, 7 }, { 6, 5 },
      { 3, 4 }, { 2, 3 }, { 1, 2 }, { 5, 1 } },
    { { 4, 5 }, { 0, 5 }, { 8, 7 }, { 7, 7 }, { 3, 6 },
      { 5, 5 }, { 2, 3 }, { 6, 2 }, { 1, 1 } },
    { { 6, 5 }, { 1, 5 }, { 5, 7 }, { 3, 7 }, { 8, 6 },
      { 7, 5 }, { 4, 3 }, { 0, 2 }, { 2, 1 } },
    { { 3, 4 }, { 1, 4 }, { 7, 7 }, { 5, 7 }, { 6, 6 },
      { 4, 5 }, { 8, 4 }, { 0, 2 }, { 2, 1 } },
    { { 0 } }, /* unused */
    { { 5, 5 }, { 4, 5 }, { 8, 7 }, { 7, 7 }, { 3, 6 },
      { 6, 5 }, { 2, 3 }, { 0, 2 }, { 1, 1 } },
    { { 5, 3 }, { 2, 3 }, { 8, 7 }, { 7, 7 }, { 4, 6 },
      { 0, 5 }, { 3, 4 }, { 6, 3 }, { 1, 1 } },
    { { 6, 4 }, { 0, 4 }, { 5, 5 }, { 4, 5 }, { 8, 6 },
      { 3, 6 }, { 7, 5 }, { 2, 2 }, { 1, 1 } },
    { { 2, 3 }, { 0, 3 }, { 8, 7 }, { 7, 7 }, { 4, 6 },
      { 6, 5 }, { 3, 4 }, { 5, 3 }, { 1, 1 } },
    { { 4, 3 }, { 2, 3 }, { 8, 7 }, { 7, 7 }, { 3, 6 },
      { 5, 5 }, { 0, 4 }, { 6, 3 }, { 1, 1 } },
    { { 3, 4 }, { 2, 4 }, { 8, 7 }, { 7, 7 }, { 0, 6 },
      { 4, 5 }, { 6, 4 }, { 5, 2 }, { 1, 1 } },
    { { 3, 7 }, { 0, 7 }, { 8, 7 }, { 7, 7 }, { 2, 5 },
      { 4, 4 }, { 5, 3 }, { 6, 2 }, { 1, 1 } },
    { { 4, 5 }, { 0, 5 }, { 8, 7 }, { 3, 7 }, { 5, 6 },
      { 7, 5 }, { 1, 3 }, { 2, 2 }, { 6, 1 } },
    { { 6, 4 }, { 0, 4 }, { 7, 7 }, { 4, 7 }, { 3, 6 },
      { 5, 5 }, { 8, 4 }, { 2, 2 }, { 1, 1 } },
    { { 2, 1 }, { 0, 1 } }, /* Unused */
    { { 1, 3 }, { 0, 3 }, { 4, 5 }, { 3, 5 }, { 6, 6 },
      { 5, 6 }, { 7, 5 }, { 8, 3 }, { 2, 1 } },
    { { 7, 4 }, { 6, 4 }, { 5, 5 }, { 3, 5 }, { 4, 6 },
      { 0, 6 }, { 8, 5 }, { 1, 2 }, { 2, 1 } },
    { { 4, 6 }, { 0, 6 }, { 5, 7 }, { 3, 7 }, { 6, 6 },
      { 1, 4 }, { 7, 3 }, { 8, 2 }, { 2, 1 } },
    { { 8, 3 }, { 1, 3 }, { 3, 4 }, { 0, 4 }, { 6, 6 },
      { 4, 6 }, { 7, 5 }, { 5, 4 }, { 2, 1 } },
    { { 7, 3 }, { 6, 3 }, { 5, 7 }, { 3, 7 }, { 0, 6 },
      { 1, 5 }, { 4, 4 }, { 8, 3 }, { 2, 1 } },
    { { 2, 2 }, { 1, 2 }, { 4, 7 }, { 0, 7 }, { 7, 6 },
      { 8, 5 }, { 6, 4 }, { 3, 3 }, { 5, 2 } },
    { { 2, 2 }, { 1, 2 }, { 7, 4 }, { 4, 4 }, { 3, 6 },
      { 0, 6 }, { 5, 5 }, { 8, 4 }, { 6, 2 } },
    { { 1, 4 }, { 0, 4 }, { 5, 7 }, { 3, 7 }, { 4, 6 },
      { 6, 5 }, { 8, 4 }, { 7, 2 }, { 2, 1 } },
    { { 7, 4 }, { 1, 4 }, { 3, 5 }, { 0, 5 }, { 5, 6 },
      { 4, 6 }, { 6, 5 }, { 8, 2 }, { 2, 1 } },
    { { 0 } }, /* unused */
    { { 1, 2 }, { 0, 2 }, { 5, 5 }, { 4, 5 }, { 7, 6 },
      { 6, 6 }, { 8, 5 }, { 3, 3 }, { 2, 2 } },
    { { 3, 3 }, { 2, 3 }, { 7, 7 }, { 0, 7 }, { 4, 6 },
      { 8, 5 }, { 6, 4 }, { 5, 3 }, { 1, 1 } },
    { { 3, 4 }, { 1, 4 }, { 6, 7 }, { 5, 7 }, { 4, 6 },
      { 7, 5 }, { 8, 4 }, { 0, 2 }, { 2, 1 } },
    { { 5, 3 }, { 2, 3 }, { 7, 7 }, { 0, 7 }, { 6, 6 },
      { 4, 5 }, { 1, 4 }, { 8, 3 }, { 3, 1 } },
    { { 2, 3 }, { 1, 3 }, { 3, 4 }, { 0, 4 }, { 7, 6 },
      { 5, 6 }, { 8, 5 }, { 6, 4 }, { 4, 1 } },
    { { 2, 4 }, { 1, 4 }, { 7, 7 }, { 0, 7 }, { 4, 6 },
      { 8, 5 }, { 6, 4 }, { 3, 2 }, { 5, 1 } },
    { { 6, 2 }, { 1, 2 }, { 5, 3 }, { 2, 3 }, { 4, 4 },
      { 3, 4 }, { 7, 5 }, { 0, 5 }, { 8, 4 } },
    { { 4, 2 }, { 2, 2 }, { 3, 3 }, { 0, 3 }, { 5, 5 },
      { 1, 5 }, { 7, 5 }, { 6, 5 }, { 8, 3 } },
    { { 2, 4 }, { 1, 4 }, { 7, 7 }, { 6, 7 }, { 0, 6 },
      { 4, 5 }, { 5, 4 }, { 3, 2 }, { 8, 1 } },
    { { 0 } }, /* unused */
    { { 1, 2 }, { 0, 2 }, { 5, 7 }, { 3, 7 }, { 8, 6 },
      { 7, 5 }, { 6, 4 }, { 4, 3 }, { 2, 2 } },
    { { 6, 3 }, { 2, 3 }, { 4, 4 }, { 0, 4 }, { 7, 6 },
      { 3, 6 }, { 8, 5 }, { 5, 4 }, { 1, 1 } },
    { { 7, 6 }, { 6, 6 }, { 5, 7 }, { 3, 7 }, { 8, 6 },
      { 1, 4 }, { 4, 3 }, { 0, 2 }, { 2, 1 } },
    { { 2, 3 }, { 0, 3 }, { 5, 4 }, { 1, 4 }, { 8, 6 },
      { 7, 6 }, { 3, 5 }, { 6, 4 }, { 4, 1 } },
    { { 5, 7 }, { 3, 7 }, { 8, 7 }, { 7, 7 }, { 1, 5 },
      { 0, 4 }, { 6, 3 }, { 2, 2 }, { 4, 1 } },
    { { 6, 2 }, { 1, 2 }, { 4, 3 }, { 2, 3 }, { 7, 6 },
      { 0, 6 }, { 3, 5 }, { 8, 4 }, { 5, 3 } },
    { { 1, 4 }, { 0, 4 }, { 8, 7 }, { 3, 7 }, { 7, 6 },
      { 5, 5 }, { 2, 4 }, { 4, 2 }, { 6, 1 } },
    { { 6, 4 }, { 0, 4 }, { 5, 7 }, { 3, 7 }, { 8, 6 },
      { 1, 5 }, { 7, 4 }, { 2, 2 }, { 4, 1 } },
    { { 2, 2 }, { 0, 2 }, { 5, 4 }, { 1, 4 }, { 6, 6 },
      { 3, 6 }, { 7, 5 }, { 8, 4 }, { 4, 2 } },
    { { 0 } }, /* unused */
    { { 2, 3 }, { 1, 3 }, { 6, 5 }, { 3, 5 }, { 7, 6 },
      { 4, 6 }, { 8, 5 }, { 5, 3 }, { 0, 1 } },
    { { 3, 4 }, { 2, 4 }, { 7, 7 }, { 0, 7 }, { 4, 6 },
      { 8, 5 }, { 6, 4 }, { 5, 2 }, { 1, 1 } },
    { { 1, 2 }, { 0, 2 }, { 6, 5 }, { 4, 5 }, { 7, 6 },
      { 3, 6 }, { 8, 5 }, { 5, 3 }, { 2, 2 } },
    { { 2, 4 }, { 1, 4 }, { 7, 7 }, { 0, 7 }, { 4, 6 },
      { 6, 5 }, { 8, 4 }, { 3, 2 }, { 5, 1 } },
    { { 2, 2 }, { 1, 2 }, { 5, 3 }, { 0, 3 }, { 4, 4 },
      { 3, 4 }, { 8, 5 }, { 7, 5 }, { 6, 4 } },
    { { 4, 7 }, { 0, 7 }, { 8, 7 }, { 7, 7 }, { 2, 5 },
      { 6, 4 }, { 3, 3 }, { 1, 2 }, { 5, 1 } },
    { { 5, 2 }, { 1, 2 }, { 8, 7 }, { 7, 7 }, { 0, 6 },
      { 4, 5 }, { 3, 4 }, { 2, 3 }, { 6, 2 } },
    { { 2, 2 }, { 1, 2 }, { 8, 3 }, { 0, 3 }, { 6, 4 },
      { 5, 4 }, { 4, 5 }, { 3, 5 }, { 7, 4 } },
    { { 2, 2 }, { 1, 2 }, { 5, 3 }, { 0, 3 }, { 7, 6 },
      { 4, 6 }, { 6, 5 }, { 3, 4 }, { 8, 3 } },
    { { 0 } }, /* unused */
    { { 2, 3 }, { 1, 3 }, { 8, 7 }, { 3, 7 }, { 5, 6 },
      { 7, 5 }, { 4, 4 }, { 6, 3 }, { 0, 1 } },
    { { 2, 4 }, { 0, 4 }, { 8, 7 }, { 3, 7 }, { 7, 6 },
      { 5, 5 }, { 4, 4 }, { 6, 2 }, { 1, 1 } },
    { { 5, 7 }, { 3, 7 }, { 8, 7 }, { 7, 7 }, { 4, 5 },
      { 1, 4 }, { 6, 3 }, { 0, 2 }, { 2, 1 } },
    { { 6, 2 }, { 1, 2 }, { 2, 3 }, { 0, 3 }, { 8, 6 },
      { 7, 6 }, { 3, 5 }, { 5, 4 }, { 4, 3 } },
    { { 5, 7 }, { 3, 7 }, { 8, 7 }, { 7, 7 }, { 1, 5 },
      { 2, 4 }, { 0, 3 }, { 4, 2 }, { 6, 1 } },
    { { 5, 3 }, { 2, 3 }, { 8, 7 }, { 7, 7 }, { 3, 6 },
      { 4, 5 }, { 0, 4 }, { 6, 3 }, { 1, 1 } },
    { { 5, 7 }, { 3, 7 }, { 8, 7 }, { 7, 7 }, { 2, 5 },
      { 0, 4 }, { 4, 3 }, { 1, 2 }, { 6, 1 } },
    { { 3, 7 }, { 0, 7 }, { 8, 7 }, { 5, 7 }, { 7, 5 },
      { 1, 4 }, { 4, 3 }, { 6, 2 }, { 2, 1 } },
    { { 2, 2 }, { 1, 2 }, { 4, 4 }, { 0, 4 }, { 7, 6 },
      { 3, 6 }, { 5, 5 }, { 8, 4 }, { 6, 2 } },
    { { 0 } }, /* unused */
    { { 6, 6 }, { 4, 6 }, { 5, 7 }, { 3, 7 }, { 8, 6 },
      { 1, 4 }, { 0, 3 }, { 7, 2 }, { 2, 1 } },
    { { 2, 2 }, { 1, 2 }, { 6, 4 }, { 0, 4 }, { 4, 6 },
      { 3, 6 }, { 5, 5 }, { 8, 4 }, { 7, 2 } },
    { { 1, 4 }, { 0, 4 }, { 5, 7 }, { 3, 7 }, { 6, 6 },
      { 4, 5 }, { 8, 4 }, { 7, 2 }, { 2, 1 } },
    { { 7, 2 }, { 2, 2 }, { 1, 3 }, { 0, 3 }, { 5, 4 },
      { 4, 4 }, { 6, 5 }, { 3, 5 }, { 8, 4 } },
    { { 4, 2 }, { 2, 2 }, { 5, 7 }, { 3, 7 }, { 8, 6 },
      { 1, 5 }, { 0, 4 }, { 6, 3 }, { 7, 2 } },
    { { 7, 2 }, { 2, 2 }, { 6, 3 }, { 1, 3 }, { 5, 4 },
      { 0, 4 }, { 4, 5 }, { 3, 5 }, { 8, 4 } },
    { { 4, 2 }, { 2, 2 }, { 3, 7 }, { 0, 7 }, { 8, 6 },
      { 5, 5 }, { 1, 4 }, { 6, 3 }, { 7, 2 } },
    { { 6, 6 }, { 1, 6 }, { 5, 7 }, { 3, 7 }, { 8, 6 },
      { 0, 4 }, { 4, 3 }, { 2, 2 }, { 7, 1 } },
    { { 8, 4 }, { 4, 4 }, { 1, 5 }, { 0, 5 }, { 5, 6 },
      { 3, 6 }, { 6, 5 }, { 7, 2 }, { 2, 1 } },
    { { 0 } }, /* unused */
    { { 8, 2 }, { 2, 2 }, { 1, 3 }, { 0, 3 }, { 6, 6 },
      { 5, 6 }, { 4, 5 }, { 7, 4 }, { 3, 3 } },
    { { 3, 3 }, { 2, 3 }, { 6, 7 }, { 0, 7 }, { 4, 6 },
      { 5, 5 }, { 7, 4 }, { 8, 3 }, { 1, 1 } },
    { { 7, 4 }, { 1, 4 }, { 3, 5 }, { 0, 5 }, { 6, 6 },
      { 5, 6 }, { 4, 5 }, { 8, 2 }, { 2, 1 } },
    { { 3, 2 }, { 2, 2 }, { 4, 6 }, { 0, 6 }, { 7, 6 },
      { 6, 6 }, { 1, 4 }, { 5, 3 }, { 8, 2 } },
    { { 8, 2 }, { 2, 2 }, { 6, 3 }, { 4, 3 }, { 5, 6 },
      { 0, 6 }, { 3, 5 }, { 1, 4 }, { 7, 3 } },
    { { 5, 2 }, { 2, 2 }, { 3, 3 }, { 1, 3 }, { 7, 6 },
      { 0, 6 }, { 4, 5 }, { 6, 4 }, { 8, 3 } },
    { { 2, 2 }, { 1, 2 }, { 6, 3 }, { 4, 3 }, { 7, 6 },
      { 0, 6 }, { 5, 5 }, { 3, 4 }, { 8, 3 } },
    { { 3, 7 }, { 0, 7 }, { 6, 7 }, { 5, 7 }, { 1, 5 },
      { 4, 4 }, { 7, 3 }, { 8, 2 }, { 2, 1 } },
    { { 1, 5 }, { 0, 5 }, { 6, 7 }, { 5, 7 }, { 4, 6 },
      { 7, 5 }, { 3, 3 }, { 2, 2 }, { 8, 1 } },
    { { 0 } }, /* unused */
};

//@}

#define PBTYPE_ESCAPE 0xFF

/** tables used for P-frame macroblock type decoding */
//@{
#define NUM_PTYPE_VLCS 7
#define PTYPE_VLC_SIZE 8
#define PTYPE_VLC_BITS 7

static const uint8_t ptype_vlc_tabs[NUM_PTYPE_VLCS][PTYPE_VLC_SIZE][2] = {
    { { PBTYPE_ESCAPE, 7 }, { 11, 7 }, { 3, 6 }, { 8, 5 },
      { 9, 4 }, { 2, 3 }, { 1, 2 }, { 0, 1 } },
    { { PBTYPE_ESCAPE, 7 }, { 3, 7 }, { 8, 6 }, { 9, 5 },
      { 11, 4 }, { 0, 3 }, { 2, 2 }, { 1, 1 } },
    { { 9, 3 }, { 8, 3 }, { 3, 4 }, { 1, 4 }, { PBTYPE_ESCAPE, 5 },
      { 0, 5 }, { 11, 4 }, { 2, 1 } },
    { { 3, 2 }, { 2, 2 }, { PBTYPE_ESCAPE, 6 }, { 11, 6 },
      { 1, 5 }, { 0, 4 }, { 8, 3 }, { 9, 2 } },
    { { PBTYPE_ESCAPE, 7 }, { 11, 7 }, { 1, 6 }, { 0, 5 },
      { 3, 4 }, { 9, 3 }, { 8, 2 }, { 2, 1 } },
    { { PBTYPE_ESCAPE, 7 }, { 11, 7 }, { 1, 6 }, { 0, 5 },
      { 3, 4 }, { 8, 3 }, { 9, 2 }, { 2, 1 } },
    { { PBTYPE_ESCAPE, 7 }, { 3, 7 }, { 0, 6 }, { 8, 5 },
      { 9, 4 }, { 1, 3 }, { 2, 2 }, { 11, 1 } },
};

static const uint8_t block_num_to_ptype_vlc_num[12] = {
 0, 1, 2, 3, 0, 0, 2, 0, 4, 5, 0, 6
};
//@}

/** tables used for P-frame macroblock type decoding */
//@{
#define NUM_BTYPE_VLCS 6
#define BTYPE_VLC_SIZE 7
#define BTYPE_VLC_BITS 6

static const uint8_t btype_vlc_tabs[NUM_BTYPE_VLCS][BTYPE_VLC_SIZE][2] = {
    { { 4, 2 }, { 0, 2 }, { PBTYPE_ESCAPE, 5 },
      { 10, 5 }, { 7, 4 }, { 1, 3 }, { 5, 2 } },
    { { PBTYPE_ESCAPE, 6 }, { 10, 6 }, { 7, 5 },
      { 0, 4 }, { 4, 3 }, { 5, 2 }, { 1, 1 } },
    { { PBTYPE_ESCAPE, 6 }, { 0, 6 }, { 10, 5 },
      { 1, 4 }, { 7, 3 }, { 5, 2 }, { 4, 1 } },
    { { 4, 3 }, { 1, 3 }, { PBTYPE_ESCAPE, 5 },
      { 0, 5 }, { 10, 4 }, { 7, 3 }, { 5, 1 } },
    { { PBTYPE_ESCAPE, 6 }, { 0, 6 }, { 1, 5 },
      { 10, 4 }, { 4, 3 }, { 5, 2 }, { 7, 1 } },
    { { PBTYPE_ESCAPE, 6 }, { 0, 6 }, { 1, 5 },
      { 10, 4 }, { 4, 3 }, { 7, 2 }, { 5, 1 } },
};

static const uint8_t block_num_to_btype_vlc_num[12] = {
 0, 1, 0, 0, 2, 3, 0, 5, 0, 0, 4, 0
};
//@}
#endif /* AVCODEC_RV40VLC2_H */
