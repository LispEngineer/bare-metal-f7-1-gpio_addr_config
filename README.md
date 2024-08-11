# Bare Metal STM32 Project

Author: [Douglas P. Fields, Jr.](mailto:symbolics@lisp.engineer)

Copyright 2024, Douglas P. Fields Jr.

License: [Apache 2.0](https://www.apache.org/licenses/LICENSE-2.0.txt)

Based upon work doing a Udemy course:
* [ARM Cortex-M7 STM32F7 Bare-Metal Programming From Ground Up](https://www.udemy.com/course/arm-cortex-m7-stm32f7-bare-metal-programming-from-ground-uptm/learn/lecture/26615904#overview)
* [Course Source](https://blog.embeddedexpert.io/?page_id=301)

See more details in code (e.g., [main.c](Src/main.c))

Tools used:
* STM32CubeIDE 1.16
* STM32CubeProgrammer
* ST Microelectronics Nucleo-F767ZI board

Eclipse plugins used:
* [Bracketeer2](https://marketplace.eclipse.org/content/bracketeer2)

GCC Options Used:
* `-save-temps` saves the generated assembly files for inspection
* `-fverbose-asm` adds more information into the assembly files

# Documentation References

* [STM32F767xx Datasheet](https://www.st.com/resource/en/datasheet/stm32f765bi.pdf)
* [RM0410 STM32F76 Reference Manual](https://www.st.com/resource/en/reference_manual/rm0410-stm32f76xxx-and-stm32f77xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
* [UM1974 Nucleo-144 User manual](https://www.st.com/resource/en/user_manual/um1974-stm32-nucleo144-boards-mb1137-stmicroelectronics.pdf)
* [Arm Cortex-M7 Technical Reference Manual](https://developer.arm.com/documentation/ddi0489/latest/)
* [Arm v7-M Architecture Reference Manual](https://developer.arm.com/documentation/ddi0403/latest/)

# Eclipse Notes

* Set "Scalability Mode" to something really high like 25,000 lines
  * Then right-click the project and rebuild the index
  * The STM header files are huge!
  
* To see the assembly code, look at these files:
  * `<filename>.s` for the raw GCC generated assembly
  * `bare-metal-f7-1-gpio_addr_config.list` to show nicely formatted assembly with
    C source code interspersed

# Lesson Notes

[Course](https://www.udemy.com/course/arm-cortex-m7-stm32f7-bare-metal-programming-from-ground-uptm/learn/lecture/26615940#overview)

## Lesson 9

Pins:
* Arranged in port
* General Purpose or Special Purpose

Busses:
* AHB - 1 clock cycle to peripherals
* APB - 2 clock cycles minimum

Clock sources
* On-chip RC (Resistor/Capacitor) - least precise
* External crystal - most precise
* PLL - programmable clock from an input clock

## Lesson 10 - Coding the GPIO Driver

Use a header file with all the addresses defined
from ST Micro site.
* st.com
* Search STM32F7
* Tools & Software
* Get STM32CubeF7
* Or get it from [GitHub](https://github.com/STMicroelectronics/STM32CubeF7)
  * This uses submodules, so after cloning, you need to:
  * `git submodule init`
  * `git submodule update`
  * Or combine those two with `git submodule update --init` and add `--recursive`
  * Or alternatively use `--recurse-submodules` in the `clone` command
* [See all GitHub Repos](https://github.com/STMicroelectronics/STM32Cube_MCU_Overall_Offer)

Look for the file `Drivers/CMSIS/Include` and `.../Drivers`
* The important one is `core_cm7.h` with the compiler headers

In `Device/ST/STM32F7xx`, look for `stm32f767xx.h`.
* This file is over 18,000 lines (!)

Also `system_stm32f7xx.h` and associated `.c` and `.s` startup files.
* We seem to be using the GCC tool chain on STM32CubeIDE

## Lessons 14-19 - UART TX

* USART3 is connected to the Nucleo USB cable
  * On APB1 per Data Sheet
  * Alternate functions: See DataSheet Rev 8 p89 Table 13
    * USART3 TX is AF7 (Alternate Function 7)
  * Per Nucleo UM, USART3 TX = PD8; RX = PD9
    * SB4-7 control ST-LINK and/or morpho; by default it is both (UM1974 Rev 10 Sec 6.9 p 26)
  * PD10-12 are CK, CTS, RTS as well
* 16 pins map to two AFRx: Alternate Function Register Low/High
  * Low is for pins 0-7
  * High is for pins 8-15
* USART3 enable is bit 18
* See RM0410 Rev 5 Chapter 34 page 1232 for more information on USART
* USART3 control registers
  * Control register 1
    * PS = even or odd parity; 0 = even
    * PCE = parity enable; 0 = off
    * M[1:0] = word length
  * Control register 2
    * # stop bits
* baud rate
  * seems to be a ratio between the usart module speed and the desired baud rate

Computer:
* See what COM port it is in PowerShell:
  * `Get-CimInstance -Class Win32_SerialPort | Select-Object Name, Description, DeviceID`
  * `COM6` in my case: `STMicroelectronics STLink Virtual COM Port`
 
After lesson 19, we have UART output via USB.