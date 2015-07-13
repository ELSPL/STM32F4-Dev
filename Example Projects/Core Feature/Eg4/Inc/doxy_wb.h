/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the Low power mode functions \n
*         -   Study the programs related to the different Low Power mode
*
* @section section2 Example1 :
*    Objective:  Write a program to enter system in Sleep mode and wake-up by using wakeup key interrupt.
*
* @section section3 Program Description:
*    This program demonstrates the Sleep mode of system and wakeup using wakeup button interrupt
*
* @section section4 Included Files:
*
*   | Header Files           	      | Source Files           	      |
*   | :----------------------------:| :----------------------------:|
*   | @ref stm32f4xx_hal_conf.h     | @ref stm32f4xx_hal_msp.c      |
*   | @ref stm32f4xx_it.h 		      | @ref stm32f4xx_it.c	 	        |
*   | @ref stm32f4_discovery.h      | @ref stm32f4_discovery.c      |
*   | @ref stm32f4_discovery_rtc.h  | @ref stm32f4_discovery_rtc.c  |
*   | @ref stm32f4_discovery_uart.h | @ref stm32f4_discovery_uart.c |
*   |                               | @ref main.c                   |
*
* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | On board(LEDs) |
*   | :------------------:| :-------------:|
*   | P1.46               | LED5           |
*   | P1.47               | LED6           |
*   | P1.12               | Push Button    |
*
* @section section6 Program Folder Location
*       <Eg1>
*
*
* @section section7 Part List
*   - STM32F4Discovery Board \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*
*
* @section section8 Hardware Configuration
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section9 Output:
*    When UART input is given the target board will enter in Sleep mode and wakeup using wakeup button.
*\n
*\n
*******************************************************************************/
