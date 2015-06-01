/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the Bsaic Timer functions \n
*         -   Study the programs related to the Basic timer
*
* @section section2 Example9 :
*    Objective: Write a program to generate melody on Buzzer using Timer as PWM. (Timer7)
*
* @section section3 Program Description:
*    This program demonstrates basic timer function by playing Mario tune using buzzer.
*
* @section section4 Included Files:
*
*   | Header Files           	        | Source Files           	       |
*   | :------------------------------:| :----------------------------: |
*   | @ref stm32f4xx_hal_conf.h       | @ref stm32f4xx_hal_msp.c       |
*   | @ref stm32f4xx_it.h 		        | @ref stm32f4xx_it.c	 	         |
*   | @ref stm32f4_discovery.h        | @ref stm32f4_discovery.c       |
*   | @ref stm32f4_discovery_timer.h  | @ref stm32f4_discovery_timer.c |
*   | @ref stm32f4_discovery_buzzer.h | @ref stm32f4_discovery_buzzer.c|
*   |                                 | @ref main.c                    |
*
* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | Peripheral |
*   | :------------------:|-----------:|
*   | GPIOC.07            | Buzzer     |
*
*
* @section section6 Program Folder Location
*       <Eg9>
*
*
* @section section7 Part List
*   - STM32F4Discovery Board \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*   - Buzzer\n
*
* @section section8 Hardware Configuration
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section9 Output:
*    The buzzer will play Mario tune.
*\n
*\n
*******************************************************************************/
