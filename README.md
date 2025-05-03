# 🚇 Passive RFID-Based Metro Card System using ESP32-S3

This project demonstrates a **contactless metro card system** using an **ESP32-S3 microcontroller** and an **MFRC522 passive RFID reader**. The system allows users to tap an RFID card, deducts fare from their virtual balance, and logs the transaction to the serial monitor.

---

## 📌 Project Overview

RFID technology is widely used in transportation systems for smart cards and ticketing. This prototype simulates such a system:

- Each RFID card has a unique ID (UID)
- A predefined balance is associated with each UID
- On tapping, ₹10 is deducted if the balance is sufficient
- The new balance or an error message is shown via Serial Monitor

This version is local-only, but easily extensible with Firebase, Google Sheets, or a web interface for recharge/admin control.

---

## 🧰 Hardware Requirements

| Component          | Specification / Notes            |
|-------------------|----------------------------------|
| ESP32-S3           | Microcontroller with Wi-Fi/BT    |
| MFRC522 RFID Reader| 13.56 MHz SPI-based module        |
| RFID Cards         | Passive 13.56 MHz ISO/IEC 14443A |
| Jumper Wires       | Male-to-male                     |
| Breadboard         | For prototyping (optional)       |
| Power Supply       | USB or 5V                        |

---

## 🔌 Wiring (ESP32-S3 to MFRC522)

| MFRC522 Pin | ESP32-S3 Pin |
|-------------|--------------|
| SDA (SS)    | GPIO21       |
| SCK         | GPIO18       |
| MOSI        | GPIO23       |
| MISO        | GPIO19       |
| RST         | GPIO22       |
| GND         | GND          |
| VCC         | 3.3V         |

> ⚠️ Note: MFRC522 works on 3.3V logic; don't use 5V unless level-shifted.

---

## 🔧 Arduino IDE Setup

1. **Install Board Manager:**
   - Go to `File > Preferences`
   - Add this to "Additional Board URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```

2. **Install MFRC522 Library:**
   - Go to `Tools > Manage Libraries`
   - Search `MFRC522 by Miguel Balboa` and install

3. **Select Board:**
   - Tools → Board → ESP32S3 Dev Module

---

## 💻 Code Functionality

- Initializes SPI and RFID
- Waits for a card to be tapped
- Converts UID to a string
- Matches the UID with the virtual card database
- Deducts fare if balance is sufficient
- Displays result on Serial Monitor

```cpp
Example Output:
Card detected: A1B2C3D4
Access Granted. Remaining Balance: ₹40
