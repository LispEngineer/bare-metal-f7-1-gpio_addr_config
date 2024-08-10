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

#include <stdint.h>

// Create a variable/register we can assign/read from an address
#define MAKE_REG(ADDR) (*(volatile unsigned int *)(ADDR))


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

#define GREEN_PIN_B 0
#define BLUE_PIN_B  7
#define RED_PIN_B   14


// How to access GPIO Port B?
// Check the memory map.
// See RM0410 Rev 5 (not in the data sheet)
// Page 78 references GPIO memory map
// GPIOB: 0x402 0400-07FF
// Part of AHB1 bus, address 0x4000 0000-7FFF
// Page 238 detailed references
// Page 185 has RCC AHB1 peripheral clock register (RCC_AHB1ENR)

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
#define RCC_OFFSET         0x00003800UL
#define RCC_BASE           (AHB1_BASE + RCC_OFFSET)
#define RCC_AHB1ENR_OFFSET 0x0030UL
// _R = Register
#define RCC_AHB1ENR_ADDR  (RCC_BASE + RCC_AHB1ENR_OFFSET)
// TODO: Shouldn't this be unsigned long given the above UL suffixes?
#define RCC_AHB1ENR_R     (MAKE_REG(RCC_AHB1ENR_ADDR))

#define GPIOB_CLK_EN      (1UL << 1) // Bit 1 of RCC_AHB1ENR_R - see page 185 of RM


// Now we have to configure the GPIO pin for direction (output in our case)
// and what data to output.
// Mode register configures the pin direction. ("MODER")
// We need to configure our three LED pins as outputs (0, 7, 14).

#define GPIOx_MODER_OFFSET 0x0000UL
#define GPIOB_MODER_ADDR   (GPIOB_BASE + GPIOx_MODER_OFFSET)
#define GPIOB_MODER_R      (MAKE_REG(GPIOB_MODER_ADDR))

// Settings for the MODER bit pairs; only bits 0-1 used
// Each pin gets 2 bits in the register.
#define MODER_INPUT  0x00UL
#define MODER_OUTPUT 0x01UL
#define MODER_ALT    0x02UL
#define MODER_ANALOG 0x03UL

// We want to be able to set any given MODER pin to any given two bits setting.
#define MODER_PIN_MASK(PIN)     (0x3UL << ((PIN) * 2))
#define MODE_PIN(PIN,MODE)      ((MODE) << ((PIN) * 2))
// The mode should be everything it was before, except these 2 pins,
// so mask off those two new pins set only them.
// newr = (oldr & mask) | setting
// TODO: Convert this to an inline function
#define MODER_PIN_SET(MODER_R,PIN,MODE) \
  do { \
    (MODER_R) = ((MODER_R) & ~(MODER_PIN_MASK(PIN))) | MODE_PIN(PIN,MODE); \
  } while (0)

// Now our output data register (ODR)
#define ODR_OFFSET     0x0014UL
#define GPIOB_ODR_ADDR (GPIOB_BASE + ODR_OFFSET)
#define GPIOB_ODR_R    (MAKE_REG(GPIOB_ODR_ADDR))
// Now set the output register - a 1 bit value only!
// TODO: Convert this to an inline function
#define ODR_PIN_SET(ODR_R,PIN,VAL) \
  do { \
    (ODR_R) = ((ODR_R) & ~(0x1UL << (PIN))) | (((VAL) & 0x01UL) << (PIN)); \
  } while (0)




int main() {

  // Enable clock access to GPIO Port B
  RCC_AHB1ENR_R |= GPIOB_CLK_EN;

  // Set our Green LED output mode
  MODER_PIN_SET(GPIOB_MODER_R, GREEN_PIN_B, MODER_OUTPUT);
  MODER_PIN_SET(GPIOB_MODER_R, BLUE_PIN_B,  MODER_OUTPUT);
  MODER_PIN_SET(GPIOB_MODER_R, RED_PIN_B,   MODER_OUTPUT);

  // Turn on and off an LED with busy waiting.
  unsigned int cur = 0;
  unsigned int pin = 0;
  const unsigned int num_pins = 3;
  while (1) {
    switch (pin) {
      case 0: ODR_PIN_SET(GPIOB_ODR_R, GREEN_PIN_B, cur); break;
      case 1: ODR_PIN_SET(GPIOB_ODR_R, BLUE_PIN_B,  cur); break;
      case 2: ODR_PIN_SET(GPIOB_ODR_R, RED_PIN_B,   cur); break;
      default: break;
    }
    cur = ~cur;
    pin++;
    if (pin >= num_pins) pin = 0;
    for (unsigned int i = 0; i < 1 * 1000 * 1000; i++);
    // The default clock must be very slow as this busy loop
    // causes like a 0.5Hz blink.
  }

  return 0; // Required by C convention, but this should never return.
}
