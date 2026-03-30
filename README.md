# 🚀 STM32 Blue Pill – Embedded Systems(Bare Metal)

Welcome to my **Embedded Systems Lab Experiments** repository!
This project showcases **core embedded systems concepts** using **bare-metal programming** on the STM32 Blue Pill.

---

## 🧠 About This Project

This repository is a collection of **hands-on experiments** designed to build a strong foundation in embedded systems.

### 💡 Key Highlights

* ✅ Written in **Embedded C**
* ✅ Uses **Direct Register Access**
* ❌ No HAL / No External Libraries (Pure Bare Metal)
* 🔍 Focus on **low-level hardware control**

```c
#include <stm32f1xx.h>
```

---

## 🧰 Tools & Setup

| Tool            | Purpose                      |
| --------------- | ---------------------------- |
| 🧰 STM32CubeIDE | Code development & debugging |
| 💻 PuTTY        | UART serial communication    |
| 🔌 USB to TTL   | Flashing & programming       |

---

## 🔌 Hardware Components

* 🔹 STM32F103C8T6 (Blue Pill)
* 🔹 LEDs + Resistors
* 🔹 Potentiometer
* 🔹 LDR Sensor
* 🔹 5V DC Motor + Driver
* 🔹 Relay Module
* 🔹 4x4 Matrix Keypad
* 🔹 16x2 LCD Display

---

## 📂 Experiments Overview

### 🔌 1. UART Communication (PC ↔ PC)

* Establishes two-way UART communication
* Verifies TX ↔ RX data transfer

---

### 💬 2. UART Message Printing

* Sends `"Hello Scholar"` from STM32 to PC
* Demonstrates basic UART transmission

---

### 🧾 3. Register Read/Write via UART

* Menu-driven interface:

  * 📖 Read register values
  * ✍️ Write to registers
* Demonstrates low-level hardware control

---

### 💡 4. LED Control Using Bitwise Operators

* Controls 8 LEDs using:

  * `AND (&)` → Turn OFF
  * `OR (|)` → Turn ON
  * `XOR (^)` → Toggle
  * `SHIFT (<< >>)` → Patterns

✨ Includes running LED effects

---

### 🎚️ 5. LED Brightness Control (ADC + PWM)

* Reads analog input from potentiometer
* Controls LED brightness using PWM

---

### ⚙️ 6. DC Motor Speed Control (PWM)

* Speed control via:

  *  PWM to the motor 
  * 💻 Serial input (25%, 50%, 75%, 100%)
* PWM verified using logic analyzer

---

### 🔁 7. Relay Control Using GPIO

* Controls relay using serial commands
* Demonstrates digital output switching

---

### 🌗 8. LDR-Based Automatic LED Control

* Detects ambient light intensity
* Behavior:

  * 🌑 Dark → LED ON
  * ☀️ Bright → LED OFF

---

### ⌨️ 9. Matrix Keypad Interfacing

* Row-column scanning technique
* Detects key press & prints via UART

---

### 📟 10. 16x2 LCD Display

* Displays:

  * 👤 Aswin Kumar A (Line 1)
  * 🏫 SSN College (Line 2)

---

## ⚙️ Concepts Covered

* 🔹 GPIO Programming
* 🔹 UART Communication
* 🔹 ADC (Analog to Digital Conversion)
* 🔹 PWM (Pulse Width Modulation)
* 🔹 Bitwise Operations
* 🔹 Register-Level Programming
* 🔹 Sensor & Peripheral Interfacing

---

## ▶️ How to Run

```bash
# Step 1: Clone the repository
git clone https://github.com/aswin0904/STM32_BAREMETAL_EXPERIMENTS
```

### 🛠️ Steps:

1. Open project in **STM32CubeIDE**
2. Build the project ⚙️
3. Connect STM32 via **USB to TTL**
4. Flash the code 🔌
5. Open **PuTTY**
6. Select **Serial Mode**
7. Set baud rate → `115200` and PORT 
8. Observe output 💻

---

## 📸 Expected Outputs

* ✅ UART messages in PuTTY
* ✅ LED patterns & brightness control
* ✅ Motor speed variation
* ✅ LCD display output
* ✅ Keypad input detection

---

## 📌 Notes

* ⚠️ This project uses **bare-metal programming only**
* 🔧 Ensure correct hardware connections
* 🔌 Common ground is **mandatory**

---

## 📬 Contact

If you have questions or suggestions, feel free to reach out!

---

⭐ **If you found this useful, don't forget to star the repo!**
