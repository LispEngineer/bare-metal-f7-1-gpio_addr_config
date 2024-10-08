/*
 * Douglas P. Fields, Jr. <symbolics@lisp.engineer>
 * August 2024
 * Copyright 2024 Douglas P. Fields, Jr.
 * License: Apache License, Version 2.0
 *          https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 * Work from: ARM Cortex-M7 STM32F7 Bare-Metal Programming From Ground Up
 * URL: https://www.udemy.com/course/arm-cortex-m7-stm32f7-bare-metal-programming-from-ground-uptm/learn/lecture/26615904#overview
 * Chapters 14-23: UART TX & TX with printf
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
#include <stdio.h>

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
#include "nucleo-uart.h"

#define GPIO_ALTERNATE_MODE (0x2U)

#define UART_DATA_8     (0x0UL)
#define UART_PARTY_NONE (0x0UL)
#define UART_STOPBITS_1 (0x0UL)

// TODO: Make these inline non-extern (compiled) functions
// Enable one or more peripheral clocks on AHB1
void set_ahb1_periph_clk(uint32_t periphs) {
  // RCC->AHB1ENR |= periphs;
  // Same thing, using STM header define
  SET_BIT(RCC->AHB1ENR, periphs); // CLEAR_BIT, MODIFY_REG, etc.
}

// Enable one or more peripheral clocks on AHB2
void set_ahb2_periph_clk(uint32_t periphs) {
  SET_BIT(RCC->AHB2ENR, periphs); // CLEAR_BIT, MODIFY_REG, etc.
}

// Enable one or more peripheral clocks on AHB1
void set_apb1_periph_clk(uint32_t periphs) {
  SET_BIT(RCC->APB1ENR, periphs); // CLEAR_BIT, MODIFY_REG, etc.
}

// Enable one or more peripheral clocks on AHB1
void set_abp2_periph_clk(uint32_t periphs) {
  SET_BIT(RCC->APB2ENR, periphs); // CLEAR_BIT, MODIFY_REG, etc.
}

// Sets the mode of an I/O pin
void set_pin_mode(GPIO_TypeDef *gpiox, uint32_t pin_num, uint32_t mode) {
  // Clear relevant bits in mode register
  gpiox->MODER &= ~(3U << (2 * pin_num));
  // Set the relevant bits in the mode register
  gpiox->MODER |= mode << (2 * pin_num);

  // Can also use MODIFY_REG() macro
  // MODIFY_REG(gpiox->MODER, (3U << (2 * pin_num)), mode << (2 * pin_num));
}

// Configures the control registers for a U(S)ART
// BUT the values must be the masked bits to set for PS, PCE, M and STOP, not the logical values!!
// (in other words, this is a stupid function that requires you to know the STM bits to set)
void config_uart_params(USART_TypeDef *usartx, uint32_t data_width, uint32_t parity, uint32_t stop_bits) {
  // The bits are defined in the STM header file
  // M is two non-contiguous bits!
  MODIFY_REG(usartx->CR1, USART_CR1_PS | USART_CR1_PCE | USART_CR1_M, parity | data_width);
  MODIFY_REG(usartx->CR2, USART_CR2_STOP, stop_bits);
}

uint16_t compute_uart_divider(uint32_t periph_clk, uint32_t desired_rate) {
  // round by adding half of a desired rate.
  // FIXME: handle 32 -> 16 bit conversion explicitly
  return (uint16_t)((periph_clk + (desired_rate / 2U)) / desired_rate);
}

void set_uart_baud_rate(USART_TypeDef *usartx, uint32_t periph_clk, uint32_t baud_rate) {
  usartx->BRR = compute_uart_divider(periph_clk, baud_rate);
}

// Enable the receiver and/or transmitter of the U(S)ART
// RM0410 Rev 5 Sec 34.8.1 p 1276
// Bits 3:2 TE:RE
void set_uart_transfer_enable(USART_TypeDef *usartx, int tx, int rx) {
  uint32_t t = 0;
  if (tx) t |= USART_CR1_TE;
  if (rx) t |= USART_CR1_RE;
  MODIFY_REG(usartx->CR1, USART_CR1_RE | USART_CR1_TE, t);
}

// This function much improved in uart3_rxtx_init()
void uart3_tx_init(void) {
  // Enable clock access for USART3 on GPIOD
  set_ahb1_periph_clk(GPIOD_CLK_EN);

  // Set PD8 mode to USART3 TX alternate function
  set_pin_mode(GPIOD, USART3_TX_PIN_D, GPIO_ALTERNATE_MODE);

  // Set the alternate function for pin 8 on GPIOD to AF7
  // Index 0 = AFR low; 1 = AFR high
  GPIOD->AFR[1] &= ~(0xFUL << 0);
  GPIOD->AFR[1] |=  (0x7UL << 0);

  // Turn on our UART clock
  set_apb1_periph_clk(USART3_CLK_EN);

  // Configure USART: 8 N 1
  config_uart_params(USART3, UART_DATA_8, UART_PARTY_NONE, UART_STOPBITS_1);

  // Configure speed: 115,200 baud
  // Note: Default clock speed is 16MHz
  set_uart_baud_rate(USART3, 16000000, 115200);

  // Enable the USART module: RM0410 Rev 5 p 1279
  USART3->CR1 |= USART_CR1_UE;

  // And enable transmission
  set_uart_transfer_enable(USART3, 1, 0); // tx only
}


// Configure the USART3 with RX and TX
void uart3_rxtx_init(void) {

  // Enable clock access for USART3 on GPIOD
  set_ahb1_periph_clk(GPIOD_CLK_EN);

  // Set up TX pin
  // Set PD8 mode to USART3 TX alternate function
  set_pin_mode(GPIOD, USART3_TX_PIN_D, GPIO_ALTERNATE_MODE);

  // Set the alternate function for pin 8 on GPIOD to AF7
  // Index 0 = AFR low; 1 = AFR high
  int afr = USART3_TX_PIN_D >= 8 ? 1 : 0;
  int shift = (USART3_TX_PIN_D - 8 * afr) * 4;
  GPIOD->AFR[afr] &= ~(0xFUL << shift);  // Clear the 4 AFR bits
  GPIOD->AFR[afr] |=  (0x7UL << shift);  // And set them to 0x7

  // Set up RX pin PD9: alternate function, and
  set_pin_mode(GPIOD, USART3_RX_PIN_D, GPIO_ALTERNATE_MODE);

  afr = USART3_RX_PIN_D >= 8 ? 1 : 0;
  shift = (USART3_RX_PIN_D - 8 * afr) * 4;
  GPIOD->AFR[afr] &= ~(0xFUL << shift);  // Clear the 4 AFR bits
  GPIOD->AFR[afr] |=  (0x7UL << shift);  // And set them to 0x7 for alternate function 7

  // Turn on our UART clock
  set_apb1_periph_clk(USART3_CLK_EN);

  // Configure USART: 8 N 1
  config_uart_params(USART3, UART_DATA_8, UART_PARTY_NONE, UART_STOPBITS_1);

  // Configure speed: 115,200 baud
  // Note: Default clock speed is 16MHz
  set_uart_baud_rate(USART3, 16000000, 115200);

  // Enable the USART module: RM0410 Rev 5 p 1279
  USART3->CR1 |= USART_CR1_UE;

  // And enable transmission
  set_uart_transfer_enable(USART3, 1, 1); // tx & rx
}

int uart_write(USART_TypeDef *usartx, uint8_t val) {
  // Ensure transmit data register is empty - "TXE"
  // If it's 1, the transmit is empty

  // the compiler optimizes this away as a "register int"
  // but not as an "int" or a "volatile register int",
  // and not if you return it
  register int x = 0;

  // I am immensely curious what x is. It presumably is related to however
  // many instructions/clocks this takes per iteration and the divider
  // (which is 16,000,000 / 115,200 ~~ 138 to 139
  while (!(usartx->ISR & USART_ISR_TXE)) x++;

  // Write the value into the transmit data register
  usartx->TDR = val;

  return x;
}


// To enable printf to send via our "console" port, we need
// to implement this function
int __io_putchar(int ch) {
  uart_write(USART3, (uint8_t)ch);
  return ch;
}


uint8_t uart_read(USART_TypeDef *usartx) {
  // Wait for read data register not empty - it will become 1
  while (!(usartx->ISR & USART_ISR_RXNE));

  // Read the receive data register and mask it to a byte
  return READ_BIT(usartx->RDR, USART_RDR_RDR) & 0xFFUL;
}

// Send stuff over ST-LINK UART
int main(void) {
  uint8_t rxc;

  uart3_rxtx_init();

  while (1) {
    printf("\r\n\r\nHello, world!\r\n");
    rxc = uart_read(USART3);
    if (rxc == 'g' || rxc == 'G') {
      printf("Goodbye, cruel world...");
    }
  }

}
