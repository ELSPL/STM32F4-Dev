/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the general Timer functions \n
*         -   Study the programs related to the General timer
*
* @section section2 Example10 :
*    Objective: Write a program to generate PWM on 4 channels in center align mode(Timer5).
*
* @section section3 Program Description:
*    This program demonstrates generation of PWM signal using general timer in center align mode.
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
*   | GPIOA.00            | TIM5_CH1   |
*   | GPIOA.01            | TIM5_CH2   |
*   | GPIOA.02            | TIM5_CH3   |
*   | GPIOA.03            | TIM5_CH4   |
*
*
* @section section6 Program Folder Location
*       <Eg10>
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
*    200HZ PWM signal will be generated on the Timer5 channel1 to channel4 port pin in center align mode.
*\n
*\n
*******************************************************************************/
