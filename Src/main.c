/*
 * Douglas P. Fields, Jr. <symbolics@lisp.engineer>
 * August, 2024
 * Copyright 2024 Douglas P. Fields, Jr.
 * License: Apache Licensee, Version 2.0
 *          https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 * Work from: ARM Cortex-M7 STM32F7 Bare-Metal Programming From Ground Up
 * URL: https://www.udemy.com/course/arm-cortex-m7-stm32f7-bare-metal-programming-from-ground-uptm/learn/lecture/26615904#overview
 *
 * My board: Nucleo-F767ZI
 * Chip: STM32F767ZIT6U
 *
 * Unfortunately, no external RAM on this board. (compare Ksoloti Core 0.6)
 *
 * Documentation links:
 * Chip reference manual (Rev 5): https://www.st.com/resource/en/reference_manual/rm0410-stm32f76xxx-and-stm32f77xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 * Board user manual (Rev 10): https://www.st.com/resource/en/user_manual/um1974-stm32-nucleo144-boards-mb1137-stmicroelectronics.pdf
 * Chip data sheet (Rev 8): https://www.st.com/resource/en/datasheet/stm32f765bi.pdf
 *
 * Other good references:
 * Mastering STM32 Book: https://www.carminenoviello.com/mastering-stm32/
 */

/*
Copyright 2024 Douglas P. Fields, Jr.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


// Nucleo-F767ZI
// Where is the LED connected? Pin? Port?
// Pins are grouped into ports.
/* UM1974 Rev 10 page 24:
 * User LD1: a green user LED is connected to the STM32 I/O
 *           PB0 (SB120 ON and SB119 OFF) (IT IS THIS ONE) or
 *           PA5 (SB119 ON and SB120 OFF) corresponding to the ST Zio D13.
 * User LD2: a blue user LED is connected to PB7.
 * User LD3: a red user LED is connected to PB14.
 */
// Green: Port B, Pin 0
// Blue: Port B, Pin 7
// Red: Port B, Pin 14

// How to access GPIO Port B?
// Check the memory map.
// See RM0410 Rev 5 (not in the data sheet)
// Page 78 references GPIO memory map
// GPIOB: 0x402 0400-07FF
// Part of AHB1 bus, address 0x4000 0000-7FFF
// Page 238 references
//

#define PERIPH_BASE  0x40000000UL // RM0410 Rev 5 page 77
#define AHB1_OFFSET  0x00020000UL // RM0410 Rev 5 page 77
#define AHB1_BASE    (PERIPH_BASE + AHB1_OFFSET)
#define GPIOB_OFFSET 0x00000400UL // RM0410 Rev 5 page 78
#define GPIOB_BASE   (AHB1_BASE + GPIOB_OFFSET)


// We have to enable the clock to the various peripherals
// before we use them - it's a power saving optimization
// (clock gating)
// Reset & Clock Control module: RCC
// RM page 78 gives this address: AHB1: 0x4002 3800 - 0x4002 3BFF
// Full details on page 218
// Offset: 0x3800
//
// Then we have to enable AHB1, because our module connects to AHB1.

// TODO: Should we mark what base each offset is against in the name?
#define RCC_OFFSET 0x00003800UL
#define RCC_BASE   (AHP1_BASE + RCC_OFFSET)

int main() { unsigned long x = GPIOB_BASE; return x; }
