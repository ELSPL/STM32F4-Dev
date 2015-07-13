/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the successive approximation analog-to-digital converter functions \n
*         -   Study the programs related to the ADC
*
* @section section2 Example9 :
*    Objective: Write a program to use Internal Temperature Sensor and Display on Terminal in Decimal value and in DegCelcius.
*
* @section section3 Program Description:
*    This program demonstrates ADC Temperature measure using ADC 1 channel 16
*
* @section section4 Included Files:
*
*   | Header Files           	     | Source Files           	    |
*   | :---------------------------:| :---------------------------:|
*   | @ref stm32f4xx_hal_conf.h    | @ref stm32f4xx_hal_msp.c     |
*   | @ref stm32f4xx_it.h 		     | @ref stm32f4xx_it.c	 	      |
*   | @ref stm32f4_discovery_adc.h | @ref stm32f4_discovery_adc.c |
*   |                              | @ref main.c                  |
*
* \n
*
*
* @section section5 Program Folder Location
*       <Eg9>
*
*
* @section section6 Part List
*   - STM32F4Discovery Board \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*
* @section section7 Hardware Configuration
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section8 Output:
*    Temperature(DegCelcius) will be shown on console window.
*\n
*\n
*******************************************************************************/
