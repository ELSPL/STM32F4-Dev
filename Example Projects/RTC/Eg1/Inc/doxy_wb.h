/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the RTC functions \n
*         -   Study the programs related to the RTC
*
* @section section2 Example 1 :
*    Objective: Write a program to set and display RTC Time and Date
*
* @section section3 Program Description:
*    This program demonstrates setup and display RTC Time and Date on UART
*
* @section section4 Included Files:
*
*   | Header Files           	        | Source Files           	       |
*   | :------------------------------:| :----------------------------: |
*   | @ref stm32f4xx_hal_conf.h       | @ref stm32f4xx_hal_msp.c       |
*   | @ref stm32f4xx_it.h 		        | @ref stm32f4xx_it.c	 	         |
*   | @ref stm32f4_discovery_uart.h   | @ref stm32f4_discovery_uart.h  |
*   | @ref stm32f4_discovery_rtc.h    | @ref stm32f4_discovery_rtc.c   |
*   |                                 | @ref main.c                    |
*
* \n
* @section section5 Program Folder Location
*       <Eg1>
*
*
* @section section6 Connection
*  | STM32F407 Reference |
*  |:-------------------:|
*  | COM1                |
*
*
* @section section7 Part List
*   - STM32F4Discovery Board \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*   - Serial Cable\n
*
*
* @section section8 Hardware Configuration
*   - Connect the board using USB port of PC using USB cable.
*   - Connect serial cable to pc and other end to COM1 on board using UART6.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section9 Output:
*    RTC time and date will be displayed on the terminal utility.
*\n
*\n
*******************************************************************************/
