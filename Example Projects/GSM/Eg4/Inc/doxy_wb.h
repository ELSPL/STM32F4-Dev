/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the GSM functions \n
*         -   Study the programs related to the GSM module
*
* @section section2 Example4 :
*    Objective: Write a program to read message from SIM card memory.
*
* @section section3 Program Description:
*    This program will read message from the SIM card memory.
*
* @section section4 Included Files:
*
*   | Header Files           	  | Source Files           	  |
*   | :------------------------:| :------------------------:|
*   | @ref stm32f4xx_hal_conf.h | @ref stm32f4xx_hal_msp.c  |
*   | @ref stm32f4xx_it.h 		  | @ref stm32f4xx_it.c	 	    |
*   | @ref stm32f4_gsm.h   	    | @ref stm32f4_gsm.c   	    |
*   |                           | @ref main.c               |
*
* \n
*
* @section section5 Connection
*   | STM32F407 Reference | Device          |
*   | :------------------:| :-------------: |
*   | COM1                | GSM Module      |
*
* @section section6 Program Folder Location
*       <Eg4>
*
*
* @section section7 Part List
*   - STM32F4Discovery Board \n
*   - Serial Cable \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*   - GSM Module \n
*   - SIM Card

*
* @section section8 Hardware Configuration
*   - Connect GSM module to educational practice board using serial cable.
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section10 Output:
*    GSM module will read the message from SIM card.
*\n
*\n
*******************************************************************************/
