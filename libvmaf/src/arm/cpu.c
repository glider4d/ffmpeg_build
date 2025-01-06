/**
 *
 *  Copyright 2016-2022 Netflix, Inc.
 *
 *     Licensed under the BSD+Patent License (the "License");
 *     you may not use this file except in compliance with the License.
 *     You may obtain a copy of the License at
 *
 *         https://opensource.org/licenses/BSDplusPatent
 *
 *     Unless required by applicable law or agreed to in writing, software
 *     distributed under the License is distributed on an "AS IS" BASIS,
 *     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *     See the License for the specific language governing permissions and
 *     limitations under the License.
 *
 */

#include "config.h"

#include "arm/cpu.h"

unsigned vmaf_get_cpu_flags_arm(void) {
    unsigned flags = 0;

#ifdef ARCH_AARCH64
    flags |= VMAF_ARM_CPU_FLAG_NEON;
#endif

    return flags;
}
