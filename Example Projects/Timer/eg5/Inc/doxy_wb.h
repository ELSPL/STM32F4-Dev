/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the general Timer functions \n
*         -   Study the programs related to the General timer
*
* @section section2 Example5 :
*    Objective: Write a Program to generate specific frequency a timer and capture it with timer in falling mode and display the frequency captured(timer4 and timer11).
*
* @section section3 Program Description:
*    This program demonstrates generation of specific frequency using general timer and capture and display it using general timer in falling edge.
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
*   | STM32F407 Reference | Peripheral |
*   | :------------------:|-----------:|
*   | GPIOD.12            | TIM4_CH1   |
*   | GPIOB.09            | TIM11_CH1  |
*
*
* @section section6 Program Folder Location
*       <Eg5>
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
*    10HZ will be generated on the Timer4 channel1 port pin and captured and displayed it with timer11 channel1 port pin in falling edge.
*\n
*\n
*******************************************************************************/
