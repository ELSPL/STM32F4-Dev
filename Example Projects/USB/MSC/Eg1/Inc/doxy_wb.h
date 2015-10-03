/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the USB Mass storage functionality \n
*         -   Study the programs related to the USB and File system
*
* @section section2 Example1 :
*    Objective: Write a program to access USB mass storage
*
* @section section3 Program Description:
*    This program demonstrates USB Mass storage functionality.
*
* @section section4 Included Files:
*
*   | Header Files           	      | Source Files           	     |
*   | :----------------------------:| :---------------------------:|
*   | @ref stm32f4xx_hal_conf.h     | @ref stm32f4xx_hal_msp.c     |
*   | @ref stm32f4xx_it.h 		      | @ref stm32f4xx_it.c	 	       |
*   | @ref stm32f4_discovery_msc.h  | @ref stm32f4_discovery_msc.c |
*	  | @ref stm32f4_discovery_uart.h | @ref stm32f4_discovery_uart.c|
*   |                               | @ref main.c                  |
*
* \n
*
* @section section5 Connection
*   | STM32F407 Reference|
*   |:------------------:|
*   | CN5                |
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
*	  - USB On-the-go(OTG) cable \n
*	  - Serial Cable
*
*
* @section section8 Hardware Configuration
*   - Connect the board using USB port of PC using USB cable.
*	  - Connect Serial cable to COM1 on target board and PC.
*     - Connect USB mass storage using USB OTG cable
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.
*
*
* @section section9 Output:
*    Write and read system log in to USB mass storage easily done.
*\n
*\n
*******************************************************************************/
