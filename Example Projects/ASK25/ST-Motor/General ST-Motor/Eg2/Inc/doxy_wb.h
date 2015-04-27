/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the GPIO functions \n
*         -   Study the programs related to the Stepper Motor.
*
* @section section2 Example2 :
*    Objective:  Program to interface of Stepper Motor and demonstrate Anti clock-wise movement.
*
* @section section3 Program Description:
*    This program demonstrates interfacing of Stepper Motor and demonstrate Anti Clock-wise movement
*
* @section section4 Included Files:
*
*   | Header Files           	  | Source Files           	  |
*   | :------------------------:| :------------------------:|
*   | @ref stm32f4xx_hal_conf.h | @ref stm32f4xx_hal_msp.c  |
*   | @ref stm32f4xx_it.h 		  | @ref stm32f4xx_it.c	 	    |
*   | @ref stm32f4_ask25.h      | @ref stm32f4_ask25.c      |
*   |                           | @ref main.c               |
*
* \n
*
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | Device(ASK-25)     |
*   | :------------------:| :----------------: |
*   | P1.27 GPIOE.9       | Stepper Motor      |
*   | P1.29 GPIOE.11      | Stepper Motor      |
*   | P1.31 GPIOE.13      | Stepper Motor      |
*   | P1.33 GPIOE.15      | Stepper Motor      |
*
*
*
* @section section6 Connection
*   | STM32F407 Reference | Device          |
*   | :------------------:| :-------------: |
*   | J6                  | ASK-25 (PL3)    |
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
*   - 9V power supply

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
*    You will see the Stepper Motor rotating in anti clockwise direction with 60 ms delay
*\n
*\n
*******************************************************************************/
