#ifdef USE_MAIN_BUTTON
/*
 * Douglas P. Fields, Jr. <symbolics@lisp.engineer>
 * August 2024
 * Copyright 2024 Douglas P. Fields, Jr.
 * License: Apache Licensee, Version 2.0
 *          https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 * Work from: ARM Cortex-M7 STM32F7 Bare-Metal Programming From Ground Up
 * URL: https://www.udemy.com/course/arm-cortex-m7-stm32f7-bare-metal-programming-from-ground-uptm/learn/lecture/26615904#overview
 * Chapter 12: GPIO Input
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

// Define our pins

// Clock enable bits
#define GPIOB_CLK_EN      (1UL << 1) // Bit 1 of RCC_AHB1ENR_R - see page 185 of RM
#define GPIOC_CLK_EN      (1UL << 2) // Bit 2 of RCC_AHB1ENR_R - see page 185 of RM

// Blue button is a user input
// Per Nucleo user guide, it's PC13 (UM1974 Rev 10 p24 sec 6.6)
// Per Datasheet, this is also on AHB1
// Per RM, we can set the GPIOC
// IDR - input data register - lower 16 bits

// Pin numbers in a bank
#define GREEN_PIN_B 0
#define BLUE_PIN_B  7
#define RED_PIN_B   14
#define USER_BTN_B  13

// Mode registers
#define USER_LED1_MODER (1U << (GREEN_PIN_B * 2)) // Output
#define USER_LED2_MODER (1U << ( BLUE_PIN_B * 2))
#define USER_LED3_MODER (1U << (  RED_PIN_B * 2))
#define USER_BTN_MODER  (~(3U << (USER_BTN_B * 2))) // Input

// Output registers
#define USER_LED1       (1U << GREEN_PIN_B)
#define USER_LED2       (1U <<  BLUE_PIN_B)
#define USER_LED3       (1U <<   RED_PIN_B)

// Input registers
#define USER_BTN        (1U << USER_BTN_B)

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
#endif
