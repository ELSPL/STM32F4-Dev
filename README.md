# STM32F407 Development 

This repository is to Capture Framework Design and Templating with approach of flexible building blocks.

## Setup Pre-requisite Changed

#### Programming:
Eclipse >= Luna (For templating and debugging to work properly)
arm-none-eabi-gcc

#### Flashing and Debugging:
Linux -> stlink flash command line utility
Win -> STM32 ST-LINK Utility
OpenOCD for stlink-v1 and v2

#### Configuration:
STM32CubeMX -> Standalone and Eclipse Plugin
X-CUBE-AI (STM32Cube.AI)
X-CUBE-BLE1
X-CUBE-MEMS1
X-CUBE-NFC4
TouchGFXDesigner -> TouchGFX graphic design and code generation tool
STemWin -> Professional graphical stack library enabling the building up of GUIs

#### Miscellaneous
Doxygen
Graphviz

---
## Requirements for Software Framework (Template)

This framework design is inspired from following Sources:
1. [IEEE SWEBOKv3](https://www.computer.org/education/bodies-of-knowledge/software-engineering)
2. [Embedded Systems Building Blocks - Jean Lebrosse](https://dl.acm.org/citation.cfm?id=582920)
3. [Software Engineering for Embedded Systems - Jean Lebrosse](https://www.oreilly.com/library/view/software-engineering-for/9780124159174/)
4. [Jack Ganssle Books](http://www.ganssle.com/book.htm)

#### Software Requirement
- Documentation
    - Doxygen and Graphviz (.doxyfile config required)
    - Template header file with comments for Documentation Main Page
    - It will convert comments to html or pdf document
    - It will also provide DAG
    - Dependency Graphs
    - Call Graphs
- Truestudio Linker file for easy memory management
- STM32CubeMX Support for easy configuration
    - Clock Config
    - Peripheral Config
    - 3rd Party Stack Config
    - DMA Config and more
- Semi-hosting support
    - printf over debug
    - RTO's debug and trace support
    - gprof and gconv support
- OTG USB support for Serial Communication 
    - Removing UART Port Dependency
    - There is always one OTG on-board USB
- OTG USB support for Storage devices
    - Implement FatFs for filesystem
- MCU Power Control
    - Switchable power control for peripherals
- MCU Clock Control
    - Systick interrupt for interval timer (Interrupt Delay support in ms,us)
    - Watchdog support
    - Sleep Mode support
    - Flexible Overclocking support
- MCU Core Features
    - Cortex-M Core Support (Stack, Memory, Registers, etc)
    - Bit-banding
    - MPU, SVC and etc
- Interrupt Services
    - Delay
    - Software Interrupt
    - Hardware Interrupt
    - Interval Interrupt
    - DMA Interrupt
- CMSIS RTO for FreeRTOS
    - Support for COM Port Debug over VCP
        - Task List
        - Runtime Stats
    - Template for Hooks (Failure Detection and Interval Status Check)
    - Filesystem and CLI support
- RTC Timer for Date and Time stamps
    - Printf implementable
- BSP Driver Files for Custom Modular Driver Development (Refer book `)`
    - It avoids modification to vendor default drivers (HAL or Legacy)
    - This will be using HAL drivers for everything
    - Modular drivers lets us manage hardware conflict and service conflict
    - Prefix for board related files will be stm32f4_discovery_xyz.c/h
    - Prefix for Custom Hardware files will be stm32f4_hardwareX_peripheralX.c/h

---

### Useful References:
1. [Jacob Beningo Articles](https://www.beningo.com/insights/articles/)
2. [Eclipse Interfacing for Embedded Development](https://mcuoneclipse.com/)