/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*          Having studied this LAB you will able to: \n
*         -   Understand the GPIO on board functions \n
*         -   Study the programs related to the various patterns of LED flashing
*
* @section section2 Example 11 :
*    Objective: Write a Program to blink board LEDs.
*
* @section section3 Program Description:
*    This program demonstrates blinking of on board LEDs.
*
* @section section4 Included Files:
*
*   | Header Files           	  | Source Files           	  |
*   | :-------------------------: | :------------------------:|
*   | @ref stm32f4xx_hal_conf.h   | @ref stm32f4xx_hal_msp.c  |
*   | @ref stm32f4xx_it.h 		  | @ref stm32f4xx_it.c	      |
*   | @ref stm32f4_discovery.h    | @ref stm32f4_discovery.c  |
*   |                             | @ref main.c               |
*
*

* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | Device (On board) |
*   | :------------------:| :---------------: |
*   | GPIOD.13            | LED3              |
*   | GPIOD.12            | LED4              |
*   | GPIOD.14            | LED5              |
*   | GPIOD.15            | LED6              |
*
*
*
* @section section7 Program Folder Location
*          <Eg11>
*
*
* @section section8 Part List
*   - STM32F4Discovery Board \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*

*
* @section section9 Hardware Configuration
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section10 Output:
*    LED4, 3, 5, 6 blinks
*\n
*\n
*******************************************************************************/
