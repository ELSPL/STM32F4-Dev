/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the SVC handler \n
*         -   Study the programs related to SVC handler and Control Register
*
* @section section2 Example1 :
*    Objective: Write a program to change privilege to unprivilege mode and vice versa.
*
* @section section3 Program Description:
*    This program demonstrates to change privilege to unprivilege mode and vice versa
*
* @section section4 Included Files:
*
*   | Header Files           	      | Source Files           	    |
*   | :----------------------------:| :--------------------------:|
*   | @ref stm32f4xx_hal_conf.h     | @ref stm32f4xx_hal_msp.c    |
*   | @ref stm32f4xx_it.h 		      | @ref stm32f4xx_it.c	 	      |
*   | @ref stm32f4_discovery.h      | @ref stm32f4_discovry.c	    |
*   | @ref stm32f4_discovery_uart.h | @ref stm32f4_discovry_uart.c|
*   |                               | @ref main.c                 |
*
* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | On board       |
*   | :------------------:| :-------------:|
*   | P1.46               | LED5           |
*   | P1.47               | LED6           |
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
*   - Connect serial cable between computer and target board.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section10 Output:
*    Change privilege mode to unprivilege mode and vice verse done using SVC handler.
*\n
*\n
*******************************************************************************/
