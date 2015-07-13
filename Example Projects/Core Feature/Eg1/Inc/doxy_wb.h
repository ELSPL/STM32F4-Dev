/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the CortexM4 Core features \n
*         -   Study the programs related to Cortexm4 core features
*
* @section section2 Example2 :
*    Objective: Write a Program to implement Bit Banding
*
* @section section3 Program Description:
*    This program demonstrates the use of Bit Banding features of CortexM4 by accessing single bit of specific region.
*
* @section section4 Included Files:
*
*   | Header Files           	  | Source Files           	  |
*   | :------------------------:| :------------------------:|
*   | @ref stm32f4xx_hal_conf.h | @ref stm32f4xx_hal_msp.c  |
*   | @ref stm32f4xx_it.h 		  | @ref stm32f4xx_it.c	 	    |
*   | @ref stm32f4_discovery.h  | @ref stm32f4_discovery.c  |
*   |                           | @ref main.c               |
*
* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | On Board       |
*   | :------------------:| :-------------:|
*   | P1.44               | LED4           |
*   | P1.13               | LED3           |
*   | P1.12               | Push Button    |
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
*    Specific bit of the address will be modify using Bit Banding feature.
*\n
*\n
*******************************************************************************/
