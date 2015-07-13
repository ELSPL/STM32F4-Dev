/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the successive approximation analog-to-digital converter functions and Timer PWM generation \n
*         -   Study the programs related to the ADC and Timer PWM generation
*
* @section section2 Example10 :
*    Objective: Write a program to use ADC conversion in interrupt mode, to vary the speed of DC motor.
*
* @section section3 Program Description:
*    This program demonstrates controlling DC Motor speed using ADC
*
* @section section4 Included Files:
*
*   | Header Files           	       | Source Files           	      |
*   | :-----------------------------:| :-----------------------------:|
*   | @ref stm32f4xx_hal_conf.h      | @ref stm32f4xx_hal_msp.c       |
*   | @ref stm32f4xx_it.h 		       | @ref stm32f4xx_it.c	 	        |
*   | @ref stm32f4_ask25.h   	       | @ref stm32f4_ask25.c   	      |
*   | @ref stm32f4_discovery_adc.h   | @ref stm32f4_discovery_adc.c   |
*   | @ref stm32f4_discovery_timer.h | @ref stm32f4_discovery_timer.c |
*   |                                | @ref main.c                    |
*
* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | Device(ASK-25) |
*   | :------------------:| :-------------:|
*   | P1.08               | PL10           |
*   | P1.15               | DC motor       |
*
* @section section6 Connection
*   | STM32F407 Reference | Device          |
*   | :------------------:| :-------------: |
*   | J1                  | ASK-25 (PL8)    |
*
* @section section7 Program Folder Location
*       <Eg10>
*
*
* @section section8 Part List
*   - STM32F4Discovery Board \n
*   - Flat cable \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*   - ASK-25 Rev2.0 \n

*
* @section section9 Hardware Configuration
*   - Connect ASK 25 to educational practice board using flat cable.
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Connect the STM32F407 Reference pin to PL10 1st pin and vary the RV2(potentiometer).
*   - Connect the Timer PWM output pin to DC motor.
*   - Observe the Output.

*
* @section section10 Output:
*    By varying the potentiometer the speed of DC motor will vary.
*\n
*\n
*******************************************************************************/
