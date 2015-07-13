/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the CortexM4 Core features \n
*         -   Study the programs related to Cortexm4 core features
*
* @section section2 Example2 :
*    Objective: Write a Program to implement Memory Protection Unit(MPU) to protect Memory region
*               which will generate fault handler on access and display output on UART.
*
* @section section3 Program Description:
*    This program demonstrates the use of MPU features of CortexM4 by setting the RW protection of Memory.
*
* @section section4 Included Files:
*
*   | Header Files           	      | Source Files           	      |
*   | :----------------------------:| :----------------------------:|
*   | @ref stm32f4xx_hal_conf.h     | @ref stm32f4xx_hal_msp.c      |
*   | @ref stm32f4xx_it.h 		      | @ref stm32f4xx_it.c	 	        |
*   | @ref stm32f4_discovery.h      | @ref stm32f4_discovery.c      |
*   | @ref stm32f4_discovery_uart.h | @ref stm32f4_discovery_uart.c |
*   |                               | @ref main.c                   |
*
* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | On Board       |
*   | :------------------:| :-------------:|
*   | P1.44               | LED4           |
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
* @section section10 Output:
*    While Accessing the protected memory region, it will generate the memory fault handler error.
*\n
*\n
*******************************************************************************/
