/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the Bsaic Timer functions \n
*         -   Study the programs related to the Basic timer
*
* @section section2 Example2 :
*    Objective: Write a Program to use Timer Match to generate specific time in interrupt mode and toggle two leds with different rate.(Timer7)
*
* @section section3 Program Description:
*    This program demonstrates basic timer function by toggling on board led with different delay.
*
* @section section4 Included Files:
*
*   | Header Files           	        | Source Files           	      |
*   | :------------------------------:| :----------------------------:|
*   | @ref stm32f4xx_hal_conf.h       | @ref stm32f4xx_hal_msp.c      |
*   | @ref stm32f4xx_it.h 		        | @ref stm32f4xx_it.c	 	        |
*   | @ref stm32f4_discovery.h        | @ref stm32f4_discovery.c      |
*   | @ref stm32f4_discovery_timer.h  | @ref stm32f4_discovery_timer.c|
*   |                                 | @ref main.c                   |
*
* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | on board       |
*   | :------------------:| :-------------:|
*   | GPIOD.13            | LED3           |
*   | GPIOD.12            | LED4           |
*
*
* @section section6 Program Folder Location
*       <Eg2>
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
* @section section9 Output:
*    LED3 and LED$ will blink with different delay.
*\n
*\n
*******************************************************************************/
