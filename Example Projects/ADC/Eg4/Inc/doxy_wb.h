/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the successive approximation analog-to-digital converter functions \n
*         -   Study the programs related to the ADC
*
* @section section2 Example4 :
*    Objective: Write a program for Dual mode ADC conversion with DMA on two groups of ADC channels(Regular Simultaneous Mode)
*
* @section section3 Program Description:
*    This program demonstrates dual mode(2 ADCs) analog to digital signal conversion using DMA with Regular Simultaneous Mode.
*
* @section section4 Included Files:
*
*   | Header Files           	     | Source Files           	    |
*   | :---------------------------:| :---------------------------:|
*   | @ref stm32f4xx_hal_conf.h    | @ref stm32f4xx_hal_msp.c     |
*   | @ref stm32f4xx_it.h 		     | @ref stm32f4xx_it.c	 	      |
*   | @ref stm32f4_ask25.h   	     | @ref stm32f4_ask25.c   	    |
*   | @ref stm32f4_discovery_adc.h | @ref stm32f4_discovery_adc.c |
*   |                              | @ref main.c                  |
*
* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | Device(ASK-25) |
*   | :------------------:| :-------------:|
*   | P1.12               | PL10           |

*
* @section section6 Connection
*   | STM32F407 Reference | Device          |
*   | :------------------:| :-------------: |
*   | J1                  | ASK-25 (PL8)    |
*
* @section section7 Program Folder Location
*       <Eg4>
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
*   - Observe the Output.

*
* @section section10 Output:
*    Converted value will be stored in ADC_CDR register and displayed on console window.
*\n
*\n
*******************************************************************************/
