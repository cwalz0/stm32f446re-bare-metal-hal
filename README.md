# STM32F446RE Bare-Metal HAL & Environmental Telemetry Node

<p align="center">
  <img src="docs/demo.gif" alt="Bare-Metal STM32 SSD1306 & AHT30 Demo" width="520" />
</p>

<p align="center">
  <em>Cold boot sequence on an STM32F446RE (ARM Cortex-M4): Custom 128x64 splash bitmap rendered over 4-wire SPI, followed by periodic I2C polling of an AHT30 sensor and live text blitting on an SSD1306 OLED.</em>
</p>

---

## Overview

A zero-dependency, bare-metal hardware abstraction layer (HAL) and environmental telemetry node written in C from scratch for the **STM32F446RE (ARM Cortex-M4)**.

This project was engineered without ST's CubeMX code generators, vendor HAL/LL libraries, or CMSIS startup templates. Every peripheral register definition, clock configuration, startup routine, vector table entry, and linker section was implemented directly from the **ST RM0390 Reference Manual** and **ARM Cortex-M4 Generic User Guide**.

---

## Firmware Architecture

The codebase is structured as a modular three-tier embedded architecture separating application logic from hardware-specific register manipulation:

### 1. Boot & Bare-Metal Peripheral Layer (`linker.ld`, `src/`, `inc/`)
* **Boot Sequence (`linker.ld`, `vector_table.c`):** Custom memory map defining FLASH and SRAM boundaries, exception vector table placement, and a bare-metal `Reset_Handler` that copies `.data` from Flash to SRAM and zero-initializes `.bss` prior to jumping to `main()`.
* **RCC (`rcc.c`, `rcc.h`):** System clock tree initialization and peripheral bus clock gating across `AHB1`, `APB1`, and `APB2`.
* **GPIO (`gpio.c`, `gpio.h`):** Pin mode configuration, alternate function (`AF`) multiplexing, output type, speed, and pull-up/pull-down resistor control.
* **SPI (`spi.c`, `spi.h`):** Register-level master-mode SPI driver polling `TXE` and `BSY` hardware status flags for high-speed display transfers.
* **I2C (`i2c.c`, `i2c.h`):** Master-mode I2C state machine handling start/stop generation, address framing, and hardware flag polling (`SB`, `ADDR`, `TXE`, `RXNE`, `BTF`).
* **USART (`usart.c`, `usart.h`):** Serial UART driver with fractional baud rate configuration for real-time host debug logging.
* **SysTick (`systick.c`, `systick.h`):** 24-bit Cortex-M4 hardware timer configured for deterministic millisecond delays and non-blocking periodic task scheduling.

### 2. Device Driver & Asset Layer
* **SSD1306 OLED Driver (`SSD1306.c`, `SSD1306.h`):** Full command/data interface over 4-wire SPI (`CS`, `DC`, `RES`), managing page/column addressing and a local 128x64 (1 KB) framebuffer.
* **Graphics Assets (`splash_bitmap.h`, `font_5x7_bitmap.h`):** Custom bit-packed boot logo (`WALZ INDUSTRIES`) and 5x7 ASCII glyph lookup table for runtime string rendering.

### 3. Application Layer (`app_*.c`, `main.c`)
* **Sensor Subsystem (`app_sensor.c`, `app_sensor.h`):** Interfaces with the **AHT30** temperature/humidity sensor over I2C (`0x38`), triggering measurement cycles, parsing 6-byte raw telemetry packets, and converting 20-bit ADC values into degrees Fahrenheit (`°F`) and relative humidity (`%`).
* **Display Subsystem (`app_display.c`, `app_display.h`):** Coordinates splash screen transitions, string formatting, and framebuffer flushes to the SSD1306 display.
* **Serial Telemetry (`app_log.c`, `app_log.h`):** Formats and streams diagnostic and environmental readings over USART.

---

## Repository Directory Structure

```text
stm32f446re-bare-metal-hal/
├── Makefile                  # Cross-compilation build rules (arm-none-eabi-gcc)
├── linker.ld                 # Custom Flash/SRAM memory layout & section definitions
├── docs/
│   └── demo.gif              # Hardware bring-up & live telemetry capture
├── inc/
│   ├── board.h               # Base memory addresses, register structs & pin mappings
│   ├── rcc.h                 # Reset and Clock Control driver interface
│   ├── gpio.h                # General-Purpose I/O driver interface
│   ├── systick.h             # Cortex-M4 SysTick timer interface
│   ├── usart.h               # Serial UART communication interface
│   ├── i2c.h                 # Inter-Integrated Circuit (I2C) driver interface
│   ├── spi.h                 # Serial Peripheral Interface (SPI) driver interface
│   ├── SSD1306.h             # SSD1306 128x64 OLED controller driver
│   ├── font_5x7_bitmap.h     # 5x7 ASCII character font table
│   ├── splash_bitmap.h       # 128x64 startup splash screen bitmap
│   ├── app_display.h         # High-level UI & display state management
│   ├── app_sensor.h          # AHT30 environmental sensor application driver
│   └── app_log.h             # Serial logging utilities
└── src/
    ├── vector_table.c        # Exception vector table & Reset_Handler startup code
    ├── main.c                # System initialization & deterministic super-loop
    ├── rcc.c                 # Clock gating & bus configuration implementation
    ├── gpio.c                # Pin multiplexing & digital I/O implementation
    ├── systick.c             # Millisecond tick counter & delay implementation
    ├── usart.c               # UART polling transmit/receive implementation
    ├── i2c.c                 # Register-level I2C master transactions
    ├── spi.c                 # Register-level SPI master transactions
    ├── SSD1306.c             # Framebuffer operations & SPI display commands
    ├── app_display.c         # Splash & live telemetry screen rendering
    ├── app_sensor.c          # AHT30 I2C command sequencing & data conversion
    └── app_log.c             # Formatted serial output implementation
```

---

## Hardware Setup

| Component | Interface | Protocol / Role |
| :--- | :--- | :--- |
| **NUCLEO-F446RE** | — | ARM Cortex-M4 MCU (`STM32F446RET6`) |
| **SSD1306 0.96" OLED** | SPI + GPIO | 4-Wire SPI (`SCK`, `MOSI`, `CS`, `DC`, `RES`) |
| **AHT30 Sensor** | I2C | 2-Wire I2C (`SCL`, `SDA` — Address `0x38`) |
| **ST-LINK V2-1** |USART2 / SWD| Virtual COM Port logging & on-chip debugging |

*(See [`inc/board.h`](inc/board.h) for exact GPIO port and alternate function pin assignments).*

---

## Build & Flash Instructions

### Prerequisites
* `arm-none-eabi-gcc` & `arm-none-eabi-binutils`
* `make`
* `stlink-tools` (`st-flash`) or `openocd`

### Compile Firmware
```bash
make clean && make
```

### Flash to Target
```bash
make flash
```
