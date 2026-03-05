# ESP32-C6 Gas Counter (MQTT / Home Assistant)

## Overview

This project implements a gas meter pulse counter using an ESP32-C6 microcontroller.
It reads pulses from a gas meter sensor (typically a reed contact or Hall sensor),
converts them into gas consumption, and publishes the data via MQTT for integration
with Home Assistant or other automation systems.

Features:

- Pulse counting with debounce
- Calculation of hourly and total energy consumption
- MQTT publishing
- WS2812 RGB LED status indicator
- Persistent storage of total consumption in NVS
- Detailed serial debugging output

The project is designed for use with:

- ESP32-C6 Zero / SuperMini boards
- Gas meters with pulse output (e.g. 0.01 m³ per pulse)
- MQTT based home automation systems

---

# Hardware

## Supported Boards

Tested with:

- Waveshare ESP32-C6 Zero
- ESP32-C6 SuperMini

Both boards provide:

- WiFi 6
- USB-CDC serial interface
- integrated WS2812 RGB LED

---

## Wiring

| Component              | ESP32 Pin           |
| ---------------------- | ------------------- |
| Gas meter pulse sensor | GPIO3               |
| WS2812 RGB LED         | GPIO8               |
| USB                    | Power + programming |

The sensor should be connected as open collector / reed contact to ground.

Recommended wiring:

3.3V
 |
 | 10k pullup (optional but recommended)
 |
GPIO3 -------- sensor -------- GND

The firmware also enables the internal pull-up resistor.

---

# How It Works

## Pulse Counting

Each pulse from the gas meter represents a fixed gas volume.

Example:
1 pulse = 0.01 m³

Energy conversion:
kWh = gas_volume_m3 × conversion_factor

Default conversion factor:
10.5 kWh per m³

---

## Debounce

Mechanical reed contacts may bounce.  
To avoid counting multiple pulses, the firmware applies a debounce time:

20 ms

Only pulses separated by more than this time are counted.

---

## Hourly Calculation

The firmware calculates:

- total consumption
- consumption during the current hour

Hourly consumption resets every:
3600 seconds (1 hour)

---

# MQTT Integration

Default MQTT broker:

192.168.1.16  
port 1883

## MQTT Topics

### Device Availability

gas_counter/availability

### Sensor State

gas_counter/state

Example payload:

{
  "gpio3":"LOW",
  "total_kwh":1234.567,
  "hour_kwh":0.231,
  "pulses_total":123456,
  "pulses_hour":22,
  "rssi":-52
}

### GPIO State

gas_counter/gpio3

Payload:
LOW / HIGH

### Debug Command Channel

gas_counter/cmd/#

---

# LED Status

| Sensor state | LED   |
| ------------ | ----- |
| LOW (pulse)  | Green |
| HIGH         | Red   |

---

# Software Setup

## Install Arduino CLI

Linux:
sudo apt install arduino-cli

Mac:
brew install arduino-cli

## Install ESP32 Board Support

arduino-cli core update-index
arduino-cli core install esp32:esp32

---

# Compile Firmware

arduino-cli compile --fqbn esp32:esp32:waveshare_esp32_c6_zero --board-options CDCOnBoot=default .

---

# Upload Firmware

arduino-cli upload --fqbn esp32:esp32:waveshare_esp32_c6_zero --board-options CDCOnBoot=default -p /dev/ttyACM0 .

---

# Serial Monitor

Linux:
tio /dev/ttyACM0 -b 115200

Mac:
tio /dev/cu.usbmodemXXXX -b 115200

---

# MQTT Debugging

mosquitto_sub -h 192.168.1.16 -t "gas_counter/#" -v

---

# Project Structure

esp32-gas-counter
│
├── Zigbee_GasCounter.ino
├── mqtt_code.md
├── install.md
├── README.md
└── .gitignore

---

# Configuration

Important parameters:

- PULSE_VOLUME_M3
- GAS_KWH_PER_M3
- MQTT_HOST
- WIFI_SSID
- WIFI_PASS

---

# License

MIT License

---

# Author

ESP32 Gas Counter Project  
Designed for DIY home automation and energy monitoring.


