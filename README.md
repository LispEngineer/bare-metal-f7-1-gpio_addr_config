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
* 
