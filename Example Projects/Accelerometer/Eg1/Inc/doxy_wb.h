/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the Accelerometer functions \n
*         -   Study the programs related to the Accelerometer
*
* @section section2 Example2 :
*    Objective: Write a program to use Accelerometer.
*
* @section section3 Program Description:
*    This program demonstrates Accelerometer functionality.
*
* @section section4 Included Files:
*
*   | Header Files           	              | Source Files                       	  |
*   | :------------------------------------:| :------------------------------------:|
*   | @ref stm32f4xx_hal_conf.h             | @ref stm32f4xx_hal_msp.c              |
*   | @ref stm32f4xx_it.h 		              | @ref stm32f4xx_it.c	 	                |
*   | @ref stm32f4_discovery_uart.h         | @ref stm32f4_discovery_uart.c         |
*   | @ref stm32f4_discovery_accelerometer.h| @ref stm32f4_discovery_accelerometer.c|
*   |                                       | @ref main.c                           |
*
* \n
*
* @section section5 Connection
*   | STM32F407 Reference |
*   | :------------------:|
*   | COM1                |
*
* @section section6 Program Folder Location
*       <Eg1>
*
*
* @section section7 Part List
*   - STM32F4Discovery Board \n
*   - Serial cable \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*
* @section section8 Hardware Configuration
*   - Connect Target board to PC using serial cable.
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section10 Output:
*    As the target board moves the value of X Y Z will change as per acceleration.
*\n
*\n
*******************************************************************************/
