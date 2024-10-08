/*
 * nucleo-clk.h
 *
 *  Created on: Aug 11, 2024
 *      Author: Douglas P. Fields, Jr.
 */

#ifndef NUCLEO_CLK_H_
#define NUCLEO_CLK_H_


// Clock enable bits on AHB1
#define GPIOB_CLK_EN      (1UL << 1) // Bit 1 of RCC_AHB1ENR_R - see page 185 of RM
#define GPIOC_CLK_EN      (1UL << 2) // Bit 2 of RCC_AHB1ENR_R - see page 185 of RM
#define GPIOD_CLK_EN      (1UL << 3) // Bit 3 of RCC_AHB1ENR_R - see page 185 of RM

// Clock enable bits on APB1 (5.3.13 p 188 of RM0410 Rev 5)
#define USART3_CLK_EN     (1UL << 18)

#endif /* NUCLEO_CLK_H_ */
