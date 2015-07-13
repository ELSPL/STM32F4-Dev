/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the RTC functions \n
*         -   Study the programs related to the RTC
*
* @section section2 Example 8 :
*    Objective: Write a program to demonstrate RTC Tamper
*
* @section section3 Program Description:
*    This program demonstrates RTC tamper feature with using time stamp
*
* @section section4 Included Files:
*
*   | Header Files           	        | Source Files           	       |
*   | :------------------------------:| :----------------------------: |
*   | @ref stm32f4xx_hal_conf.h       | @ref stm32f4xx_hal_msp.c       |
*   | @ref stm32f4xx_it.h 		        | @ref stm32f4xx_it.c	 	         |
*   | @ref stm32f4_discovery_rtc.h    | @ref stm32f4_discovery_rtc.c   |
*   | @ref stm32f4_discovery_uart.h   | @ref stm32f4_discovery_uart.h  |
*   | @ref stm32f4_discovery.h        | @ref stm32f4_discovery.c       |
*   |                                 | @ref main.c                    |
*
*
* @section section5 Pin Assignments
*
*  | STM32F407 Reference |
*  | :------------------:|
*  | P2.12               |
*
*
* \n
* @section section6 Program Folder Location
*       <Eg8>
*
* @section section7 Connection
*  | STM32F407 Reference |
*  |:-------------------:|
*  | COM1                |
*
*
* @section section8 Part List
*   - STM32F4Discovery Board \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*   - Serial Cable\n
*
*
* @section section9 Hardware Configuration
*   - Connect the board using USB port of PC using USB cable.
*   - Connect serial cable to PC and other end to COM1 on board using UART6.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section10 Output:
*    RTC time stamp interrupt will be generated when an event is detected on pin PC13 and all the Backup register will be erased and time will stored in Time stamp register.
*\n
*\n
*******************************************************************************/
