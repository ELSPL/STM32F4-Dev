/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the Core Feature \n
*         -   Study the programs related to the memory protection unit
*
* @section section2 Example2 :
*    Objective: Write a Program to implement MPU to protect memory region 4 (on-chip SRAM) which will generate fault handler on access and display output.
*
* @section section3 Program Description:
*    This program demonstrates Memory management using MPU feature by accessing the different memory region.
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
*
* @section section5 Program Folder Location
*       <Eg2>
*
*
* @section section6 Part List
*   - STM32F4Discovery Board \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*
*
* @section section7 Hardware Configuration
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.
*
*
* @section section8 Output:
*    Region 0,1,3 can be accessible and when you try to access the region 2 it will generate memory fault handler.
*\n
*\n
*******************************************************************************/
