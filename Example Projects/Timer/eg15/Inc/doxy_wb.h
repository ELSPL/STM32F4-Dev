/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the general Timer functions \n
*         -   Study the programs related to the general timer
*
* @section section2 Example16 :
*    Objective: Write a Program to generate PWM signal and capture it and display the frequency captured and duty cycle(Timer12)
*
* @section section3 Program Description:
*    This program demonstrates generation PWM signal using general timer and capture and display it using general timer.
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
*   | GPIOA.05            | TIM2_CH1   |
*   | GPIOB.14            | TIM12_CH1  |
*
*
* @section section6 Program Folder Location
*       <Eg16>
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
*    2KHZ PWM signal will be captured on Timer12 channel1 and captured frequency and duty cycle displayed.
*\n
*\n
*******************************************************************************/
