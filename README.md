# TM4C_BareMetal

**Self OS — ARM Cortex-M4 Bare-Metal Programming (TM4C1294NCPDT)**

![ARM](https://img.shields.io/badge/Arch-ARM_Cortex--M4-red)
![C](https://img.shields.io/badge/Language-C-blue)
![Assembly](https://img.shields.io/badge/Language-Assembly-lightgrey)
![IDE](https://img.shields.io/badge/IDE-Keil_uVision-green)
![License](https://img.shields.io/badge/License-MIT-yellow)

---

## 📖 Overview

This repository contains a complete, step-by-step bare-metal programming course for the **Texas Instruments TM4C1294NCPDT** microcontroller (ARM Cortex-M4F). Starting from nothing — no HAL, no SDK — building everything from the ground up: startup code, GPIO, interrupts, timers, ADC, SPI, UART, and more.

> **"Self OS"** — because we build everything ourselves, from the vector table to the peripheral drivers.

---

## 📦 Repository Structure

```
TM4C_BareMetal/
├── 00_cnt_debug/          # Debug setup & counter examples
├── 01_cnt_debug/          # Advanced debug techniques
├── 02_LED_on/             # First bare-metal LED blink
├── 03_Swicth_Control/     # GPIO input with switch
├── 04_export_from_Assembly/# Mixing Assembly & C
├── 05_export_from_C/
├── 06_var_in_Assembly/
├── 07_Embedded_Essential/ # Essential embedded concepts
├── 08_Keil_CMSIS/         # CMSIS core introduction
├── 09_Keil_CMSIS_IO/
├── 10_Keil_CMSIS_IO_hw/
├── 11_GPIO_Driver/        # Custom GPIO driver from scratch
├── 12_LCD_Basic/          # Character LCD interfacing
├── 13_seven_segment/      # 7-segment display
├── 14_four_sevSegment/    # Multiplexed 4-digit display
├── 15_LCD_fourbit/        # 4-bit LCD mode
├── 16_KeyPadwLCD/         # Matrix keypad + LCD
├── 17_Systick_Timer/      # SysTick configuration
├── 18_GPTM_oneshot/       # General-purpose timer (one-shot)
├── 19_GPTM_periodic/      # GPTM periodic mode
├── 20_GPTM_cnt_RisEdges/  # Input edge counting
├── 21_GPTM_measure_b_Events/ # Event measurement
├── 22_GPTM_wideTimers/    # 64-bit wide timers
├── 23_Systick_ISR/        # SysTick interrupt handler
├── 24_GPIO_IRQ/           # GPIO external interrupts
├── 25_GPIO_multi_IRQ/     # Multiple interrupt sources
├── 26_Time_IRQ/           # Timer interrupts
├── 27_ADC_basic/          # Analog-to-digital converter
├── 28_ADC_TimerTrigger/   # Timer-triggered ADC
├── 29_SPI_basic/          # SPI communication
├── 30_SPI_ADC_5110Screen/ # SPI + ADC + Nokia 5110 LCD
├── 31_UART_driver_assembly/ # UART driver in Assembly
├── GPIO_Test/             # GPIO test bench
├── includes/              # Shared headers
├── _extra/                # Supplementary materials
├── LICENSE
└── README.md
```

---

## 🚀 Topics Covered

| Category | Topics | # Lessons |
|:---------|:-------|:---------:|
| **Getting Started** | Debug setup, toolchain, first blink | 2 |
| **GPIO** | LED, switch, custom driver, interrupts | 5 |
| **Timers** | SysTick, GPTM (one-shot, periodic, capture) | 6 |
| **Interrupts** | SysTick IRQ, GPIO IRQ, timer IRQ, multi-IRQ | 4 |
| **Display** | 7-segment, LCD (8-bit & 4-bit), matrix keypad | 5 |
| **ADC** | Basic ADC, timer-triggered conversion | 2 |
| **Serial Comms** | SPI, SPI+ADC+Nokia 5110, UART in Assembly | 3 |
| **Low-Level** | Startup code, vector table, Assembly/C interop | 4 |
| **CMSIS** | ARM CMSIS core, I/O, hardware abstraction | 3 |

---

## 🛠 Hardware & Tools

| Component | Detail |
|:----------|:-------|
| **MCU** | TM4C1294NCPDT (ARM Cortex-M4F @ 120 MHz) |
| **Board** | Texas Instruments EK-TM4C1294XL |
| **IDE** | Keil MDK-ARM (uVision) |
| **Debugger** | On-board ICDI |
| **Language** | C, ARM Assembly (Thumb-2) |

---

## 📋 Prerequisites

- **IDE:** Keil MDK-ARM v5+ (or any ARM-compatible toolchain)
- **Board:** EK-TM4C1294XL LaunchPad (or similar TM4C1294 board)
- **Knowledge:** Basic C, basic computer architecture

---

## 🚦 Quick Start

```bash
# Clone the repository
git clone https://github.com/mustafaylmz1995/TM4C_BareMetal.git
cd TM4C_BareMetal

# Open any lesson in Keil uVision
# e.g., 02_LED_on/LED_on.uvprojx
```

---

## 📄 License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file.

---

## 📬 Contact

**Mustafa YILMAZ** — Embedded Software Engineer

[![LinkedIn](https://img.shields.io/badge/LinkedIn-Connect-blue)](https://linkedin.com/in/mustafaylmz1995)
[![GitHub](https://img.shields.io/badge/GitHub-Follow-black)](https://github.com/mustafaylmz1995)
