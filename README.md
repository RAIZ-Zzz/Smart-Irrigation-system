# 🌱 ESP32 Smart Irrigation System

![License](https://img.shields.io/badge/license-MIT-blue.svg) ![Platform](https://img.shields.io/badge/platform-ESP32-green.svg) ![Status](https://img.shields.io/badge/status-active-success.svg)

> An intelligent, automated irrigation solution based on ESP32-S3 and MQTT.

## Introduction
This project utilizes the **ESP32-S3 microcontroller** to automatically irrigate plants when dry. It supports remote monitoring and control via the **MQTT protocol**, making it an ideal IoT solution for smart gardening. Edge computing will also be applied to decide the irrigation process.

**Key Features:**
-  Remote data reporting via MQTT.
-  Automatic valve control.
-  Low-power consumption design.
-  Edge computing engaged. 

## Hardware Requirements
- **Microcontroller**: ESP32-S3
- **Actuators**: 6V valves, FM-PS2216 Optical water volume flow sensor, FS-IR82B water level detector.
- **Others**: Breadboard, Jumper wires, Power supply

##  Software Prerequisites
- **ESP-IDF** v4.4 or later
- **VS Code** (with Espressif IDF Extension)

##  Getting Started

### 1. Clone the Repository
```bash
git clone [https://github.com/YOUR-USERNAME/esp32-smart-irrigation.git](https://github.com/YOUR-USERNAME/esp32-smart-irrigation.git)
cd esp32-smart-irrigation

### 2. Commit name rule:
- feat (Feature): 
feat(mqtt): add connect retry logic 

- fix (Fix): 
fix(sensor): resolve DHT11 reading error 

- docs (Documentation): 
docs: update README with wiring diagram 

- style (Style):
style: format main.c code

- refactor (Refactoring): 
refactor(wifi): optimize connection function

- chore (Chore): 
chore: update .gitignore to exclude build folder