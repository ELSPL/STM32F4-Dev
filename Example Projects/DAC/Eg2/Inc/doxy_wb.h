/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the DAC functions \n
*         -   Study the programs related to the DAC conversion
*
* @section section2 Example2 :
*    Objective: Write a program to Generate 1 V using DAC.
*
* @section section3 Program Description:
*    This program demonstrates generation of 1V using DAC
*
* @section section4 Included Files:
*
*   | Header Files           	  | Source Files           	  |
*   | :------------------------:| :------------------------:|
*   | @ref stm32f4xx_hal_conf.h | @ref stm32f4xx_hal_msp.c  |
*   | @ref stm32f4xx_it.h 		  | @ref stm32f4xx_it.c	 	    |
*   |                           | @ref main.c               |
*
* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | Oscilloscope   |
*   | :------------------:| :-------------:|
*   | P1.15               |  Channel(CH2)  |
*
*
* @section section6 Program Folder Location
*       <Eg2>
*
*
* @section section7 Part List
*   - STM32F4Discovery Board \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*   - Oscilloscope \n
*
*
* @section section8 Hardware Configuration
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Connect Oscilloscope to STM32F407 Reference pin.
*   - Observe the Output.

*
* @section section10 Output:
*    1V will be generated and displayed on Oscilloscope.
*\n
*\n
*******************************************************************************/
