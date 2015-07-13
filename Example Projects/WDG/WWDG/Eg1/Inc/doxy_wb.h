/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the window Watchdog timer functions \n
*         -   Study the programs related to the Watchdog timer
*
* @section section2 Example1 :
*    Objective: Write a program to understand window watch dog timer.
*
* @section section3 Program Description:
*    This program demonstrates the use of window watchdog timer by accessing reserved memory.
*
* @section section4 Included Files:
*
*   | Header Files           	      | Source Files           	      |
*   | :----------------------------:| :----------------------------:|
*   | @ref stm32f4xx_hal_conf.h     | @ref stm32f4xx_hal_msp.c      |
*   | @ref stm32f4xx_it.h 		      | @ref stm32f4xx_it.c	 	        |
*   | @ref stm32f4_discovery.h      | @ref stm32f4_discovery.c      |
*   | @ref stm32f4_discovery_wwdg.h | @ref stm32f4_discovery_wwdg.c |
*   |                               | @ref main.c                   |
*
* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | Device (On board) |
*   | :------------------:| :---------------: |
*   | GPIOD.14            | LED5              |
*   | GPIOD.12            | LED4              |
*   | GPIOA.0             | Switch B1         |
*
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
* @section section8 Hardware Configuration
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section10 Output:
*    When Switch B1 will be pressed the target board will be reset due to window watchdog timer.
*\n
*\n
*******************************************************************************/
