/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the UART functions \n
*         -   Study the programs related to UART
*
* @section section2 Example1 :
*    Objective: Write a program to transmit a single character using UART6 (Polling).
*
* @section section3 Program Description:
*    This program transmits single character using UART6
*
* @section section4 Included Files:
*
*   | Header Files           	      | Source Files           	     |
*   | :----------------------------:| :---------------------------:|
*   | @ref stm32f4xx_hal_conf.h     | @ref stm32f4xx_hal_msp.c     |
*   | @ref stm32f4xx_it.h 		      | @ref stm32f4xx_it.c	 	       |
*   | @ref stm32f4_discovery_uart.h | @ref stm32f4_discovery_uart.c|
*   |                               | @ref main.c                  |
*
* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | Interface      |
*   | :------------------:| :-------------:|
*   | P2.47 PC.6          | USART6_TX      |
*   | P2.48 PC.7          | USART6_RX      |
*
* @section section6 Connection
*   | STM32F407 Reference |
*   |:-------------------:|
*   | COM1                |
*
* @section section7 Program Folder Location
*       <Eg1>
*
*
* @section section8 Part List
*   - STM32F4Discovery Board \n
*   - USB cable \n
*   - Serial cable\n
*   - Eclipse IDE \n
*   - PC \n
*   - Hyper terminal\n

*
* @section section9 Hardware Configuration
*   - Connect the board using USB port of PC using USB cable.
*   - Connect serial cable to pc and other end to COM1 on board using UART6.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section10 Output:
*    Character will be displayed on hyper terminal utility.
*\n
*\n
*******************************************************************************/
