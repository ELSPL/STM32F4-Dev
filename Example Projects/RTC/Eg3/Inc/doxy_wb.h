/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the RTC functions \n
*         -   Study the programs related to the RTC
*
* @section section2 Example 3 :
*    Objective: Write a program to configure Date from user and display
*
* @section section3 Program Description:
*    This program demonstrates configuration of RTC Date and display on UART
*
* @section section4 Included Files:
*
*   | Header Files           	        | Source Files           	       |
*   | :------------------------------:| :----------------------------: |
*   | @ref stm32f4xx_hal_conf.h       | @ref stm32f4xx_hal_msp.c       |
*   | @ref stm32f4xx_it.h 		        | @ref stm32f4xx_it.c	 	         |
*   | @ref stm32f4_discovery_rtc.h    | @ref stm32f4_discovery_rtc.c   |
*   | @ref stm32f4_discovery_uart.h   | @ref stm32f4_discovery_uart.h  |
*   |                                 | @ref main.c                    |
*
* \n
* @section section5 Program Folder Location
*       <Eg3>
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
*   - Connect serial cable to PC and other end to COM1 on board using UART6.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section9 Output:
*    RTC date configured properly and displayed on UART.
*\n
*\n
*******************************************************************************/
