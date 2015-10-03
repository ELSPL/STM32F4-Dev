/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the Camera Functions \n
*         -   Study the programs related to Camera and LCD interface
*
* @section section2 Example1 :
*    Objective: Write a program to demonstrate camera function.
*
* @section section3 Program Description:
*    This program demonstrates different camera functions like contrast and
*    brightness control, mirror and flip the image.
*
* @section section4 Included Files:
*
*   | Header Files           	       | Source Files           	      |
*   | :-----------------------------:| :-----------------------------:|
*   | @ref stm32f4xx_hal_conf.h      | @ref stm32f4xx_hal_msp.c       |
*   | @ref stm32f4xx_it.h 		       | @ref stm32f4xx_it.c	 	        |
*   | @ref stm32f4_discovery_lcd.h   | @ref stm32f4_discovery_lcd.c   |
*   | @ref stm32f4_discovery_camera.h| @ref stm32f4_discovery_camera.c|
*   |                                | @ref main.c                    |
*
* \n
* @section section5 Connection
*   | STM32F407 Reference |
*   | :------------------:|
*   | COM7(DCMI)          |
*   | COM3(LCD)           |
*
* @section section6 Program Folder Location
*       <Eg1>
*
*
* @section section7 Part List
*   - STM32F4Discovery Board \n
*   - Camera \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*   - LCD Display \n

*
* @section section9 Hardware Configuration
*   - Connect Camera and LCD display to educational practice board.
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section10 Output:
*    All Camera function is working properly.
*\n
*\n
*******************************************************************************/
