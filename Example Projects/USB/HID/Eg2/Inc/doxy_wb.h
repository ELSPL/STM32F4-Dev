/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the USB Host interface functionality \n
*         -   Study the programs related to the USB and Human Interface Devices(HID).
*
* @section section2 Example1 :
*    Objective: Write a program to use HIDs via USB
*
* @section section3 Program Description:
*    This program demonstrates USB Host interface functionality.
*
* @section section4 Included Files:
*
*   | Header Files           	      | Source Files           	     |
*   | :----------------------------:| :---------------------------:|
*   | @ref stm32f4xx_hal_conf.h     | @ref stm32f4xx_hal_msp.c     |
*   | @ref stm32f4xx_it.h 		      | @ref stm32f4xx_it.c	 	       |
*   | @ref stm32f4_discovery_hid.h  | @ref stm32f4_discovery_hid.c |
*	  | @ref stm32f4_discovery_lcd.h  | @ref stm32f4_discovery_lcd.c |
*   |                               | @ref main.c                  |
*
* \n
*
* @section section5 Connection
*   | STM32F407 Reference|
*   |:------------------:|
*   | CN5                |
*   | LCD                |
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
*   - Mouse
*
*
* @section section8 Hardware Configuration
*   - Connect the board using USB port of PC using USB cable.
*	  - Connect Mouse using USB OTG cable
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.
*
*
* @section section9 Output:
*    Mouse is Properly working and can accessible using LCD display.
*\n
*\n
*******************************************************************************/
