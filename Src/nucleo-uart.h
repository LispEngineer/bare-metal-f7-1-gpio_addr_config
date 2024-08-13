/*
 * nucleo-uart.h
 *
 *  Created on: Aug 11, 2024
 *      Author: Douglas P. Fields, Jr.
 */

#ifndef NUCLEO_UART_H_
#define NUCLEO_UART_H_

/*

* USART3 is connected to the Nucleo USB cable
  * On APB1 per Data Sheet
  * Alternate functions: See DataSheet Rev 8 p89 Table 13
  * Per Nucleo UM, USART3 TX = PD8; RX = PD9
    * SB4-7 control ST-LINK and/or morpho; by default it is both (UM1974 Rev 10 Sec 6.9 p 26)
  * PD10-12 are CK, CTS, RTS as well

 */

// USART3 transmit data pin on bank D alternate mode
#define USART3_TX_PIN_D 8
#define USART3_RX_PIN_D 9

#endif /* NUCLEO_UART_H_ */
