/*
 * Douglas P. Fields, Jr. <symbolics@lisp.engineer>
 * August 2024
 * Copyright 2024 Douglas P. Fields, Jr.
 * License: Apache Licensee, Version 2.0
 *          https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 * Work from: ARM Cortex-M7 STM32F7 Bare-Metal Programming From Ground Up
 * URL: https://www.udemy.com/course/arm-cortex-m7-stm32f7-bare-metal-programming-from-ground-uptm/learn/lecture/26615904#overview
 * Chapters 15: UART TX
 *
 * My board: Nucleo-F767ZI
 * Chip: STM32F767ZIT6U
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

// We need to set include files from STM32CubeF7
//   Project -> Properties -> C/C++ General -> Paths and Symbols
//   Add two directories: CMSIS/Include and CMSIS/Device/.../Include
// Then we have to set the device type to be included:
//   Project -> Properties -> C/C++ Build -> Settings -> Tool Settings ->
//     MCU/MPU GCC Compiler -> Preprocessor -> Define symbols -D -> Add
//   STM32F767xx
//   (Several others were already defined, e.g. STM32F767ZITx)
#include "stm32f7xx.h"

#include "nucleo-btn.h"
#include "nucleo-leds.h"
#include "nucleo-clk.h"

int main(void) {
  // Enable clock access to Ports B & C
  RCC->AHB1ENR |= GPIOB_CLK_EN | GPIOC_CLK_EN;

  // Configure LED pins as output pins
  GPIOB->MODER |= USER_LED1_MODER | USER_LED2_MODER | USER_LED3_MODER;

  // Configure button pin as input pin
  GPIOC->MODER &= USER_BTN_MODER;

  while (1) {
    // Toggle LEDs when button not pushed
    if (!(GPIOC->IDR & USER_BTN)) {
      GPIOB->BSRR = USER_LED1 | USER_LED2 | USER_LED3; // Turn LEDs on
      for (int i = 0; i < 1000000; i++);
    }

    if (!(GPIOC->IDR & USER_BTN)) {
      GPIOB->BSRR = (USER_LED1 | USER_LED2 | USER_LED3) << 16; // Turn LEDs off
      for (int i = 0; i < 1000000; i++);
    }
  }
}
