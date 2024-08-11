/*
 * nucleo-clk.h
 *
 *  Created on: Aug 11, 2024
 *      Author: Douglas P. Fields, Jr.
 */

#ifndef NUCLEO_CLK_H_
#define NUCLEO_CLK_H_


// Clock enable bits
#define GPIOB_CLK_EN      (1UL << 1) // Bit 1 of RCC_AHB1ENR_R - see page 185 of RM
#define GPIOC_CLK_EN      (1UL << 2) // Bit 2 of RCC_AHB1ENR_R - see page 185 of RM


#endif /* NUCLEO_CLK_H_ */
