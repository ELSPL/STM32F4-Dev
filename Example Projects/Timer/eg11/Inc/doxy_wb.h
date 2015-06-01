/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the advanced Timer functions \n
*         -   Study the programs related to the Advanced timer
*
* @section section2 Example11 :
*    Objective: Write a program to generate complimentary PWM output in center align mode without deadtime(Timer1)
*
* @section section3 Program Description:
*    This program demonstrates generation of complimentary PWM signal using advanced timer in center align mode.
*
* @section section4 Included Files:
*
*   | Header Files           	        | Source Files           	       |
*   | :------------------------------:| :----------------------------: |
*   | @ref stm32f4xx_hal_conf.h       | @ref stm32f4xx_hal_msp.c       |
*   | @ref stm32f4xx_it.h 		        | @ref stm32f4xx_it.c	 	         |
*   | @ref stm32f4_discovery.h        | @ref stm32f4_discovery.c       |
*   | @ref stm32f4_discovery_timer.h  | @ref stm32f4_discovery_timer.c |
*   |                                 | @ref main.c                    |
*
* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | On board   |
*   | :------------------:|-----------:|
*   | GPIOE.09            | TIM1_CH1   |
*   | GPIOA.07N           | TIM1_CH1N  |
*
*
* @section section6 Program Folder Location
*       <Eg11>
*
*
* @section section7 Part List
*   - STM32F4Discovery Board \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*
*
* @section section8 Hardware Configuration
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section9 Output:
*    10KHZ complimentary PWM signal will be generated on the Timer1 channel1N port pin in center align mode.
*\n
*\n
*******************************************************************************/
