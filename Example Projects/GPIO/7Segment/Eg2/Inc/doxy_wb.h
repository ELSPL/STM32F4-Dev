/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the GPIO functions \n
*         -   Study the programs related to the 7 Segment interface
*
* @section section2 Example2 :
*    Objective: Write a program to display digits 0 to 9 on 7segment display on Seg1.
*
* @section section3 Program Description:
*    This program demonstrates 7 segment interface and display 0 to 9 digit on it. 
*
* @section section4 Included Files:
*
*   | Header Files           	  | Source Files           	  |
*   | :------------------------:| :------------------------:|
*   | @ref stm32f4xx_hal_conf.h | @ref stm32f4xx_hal_msp.c  |
*   | @ref stm32f4xx_it.h 		  | @ref stm32f4xx_it.c	 	    |
*   | @ref stm32f4_ask25.h   	  | @ref stm32f4_ask25.c   	  |
*   |                           | @ref main.c               |
*
*
*
* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference  | Device (ASK25) |
*   | :------------------: | :-------------:|
*   |  GPIO B.4(P2.25)     |7Segment select |
*   |  GPIO B.5(P2.26)     | Gate pin       |
*   |  GPIO E.8(P1.26)     | Data pin       |
*   |  GPIO E.9(P1.27)     | Data pin       |
*   |  GPIO E.10(P1.28)    | Data pin       |
*   |  GPIO E.11(P1.29)    | Data pin       |
*   |  GPIO E.12(P1.30)    | Data pin       |
*   |  GPIO E.13(P1.31)    | Data pin       |
*   |  GPIO E.14(P1.32)    | Data pin       |
*   |  GPIO E.15(P1.33)    | Data pin       |
*
* @section section6 Connection
*   | STM32F407 Reference | Device          |
*   | :------------------:| :-------------: |
*   | J6                  | ASK-25 (PL8)    |
*
* @section section7 Program Folder Location
*       <Eg2>
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
*   - Observe the Output.

*
* @section section10 Output:
*    On first 7 segment you will see the digit 0 to 9.
*\n
*\n
*******************************************************************************/
