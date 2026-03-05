![ESP32-C6 Super Mini image](https://www.espboards.dev/img/dO0MxEDpD1-300.png)

# ESP32-C6 Super Mini

Development Board

Code name: ESP32C6_DEV

[ESP32C6](https://www.espboards.dev/esp32/microcontroller/esp32c6/)[SUPERMINI](https://www.espboards.dev/esp32/type/supermini/)

ESP32-C6 Super Mini is a development board based on the [ESP32C6 microcontroller ](https://www.espboards.dev/blog/esp32-soc-options/)using RISCV32 architecture.

This board features a maximum CPU frequency of 160 MHz and 4MB flash memory.

## About ESP32-C6 Super Mini



The **ESP32-C6 SuperMini** is a compact and powerful development board featuring the **Espressif ESP32-C6** WiFi 6 and Bluetooth dual-mode chip. With support for **IEEE 802.11ax WiFi 6 (2.4 GHz)** and **Bluetooth 5 (LE) with Bluetooth Mesh**, it ensures fast and reliable wireless connectivity.

At its core, the ESP32-C6 SuperMini is powered by a **RISC-V 32-bit single-core processor** running at up to 160 MHz, offering efficient performance for IoT applications. It includes **512 KB of high-performance SRAM**, **16 KB of low-power SRAM**, and **4 MB of flash memory**, giving your projects plenty of space to run smoothly.

Designed with ease of use in mind, the board comes with a **USB Type-C interface** for simple programming and connectivity. Plus, its **compact form factor** makes it ideal for space-constrained applications.

With **versatile interfaces** (UART, I2C, SPI) and a rich set of GPIOs, the **ESP32-C6 SuperMini** is a great choice.

🆚 Wondering how the **ESP32-S3 SuperMini** compares to other SuperMini boards? Check out our [full comparison guide](https://www.espboards.dev/blog/esp32-super-mini-comparison/) to see how it stacks up against the C3, C3 Plus, C6, and H2.

## Technical Specifications

Complete technical specification details for ESP32-C6 Super Mini

### Connectivity

WiFi802.11 ax b/g/n (2.4 GHz)

Bluetooth5.3

BLE5.3

### Microcontroller

Modelesp32c6

Clock Speed160 MHz

Flash Size4MB

Architectureriscv32

### ✨Features & Pins

Digital IO11

Analog Input6

PWM11

Interrupts22

- •Ultra-small size for embedded applications

- •Ultra-low power consumption with low-power working modes

- •RISC-V 32-bit single-core CPU running at up to 160 MHz

- •512 KB high-performance SRAM and 16 KB low-power SRAM

- •WiFi 6 (802.11ax, 2.4 GHz) with 40 MHz bandwidth support

- •Bluetooth 5 (LE) and Bluetooth Mesh support

- •USB Type-C interface for programming and connectivity

- •Stamp hole design for direct soldering onto PCB

  ## Quick Setup

  Copy-paste configs for **ESP32-C6 Super Mini** - auto‑generated from this board's exact hardware specs.

  ESP32C6160 MHz4MBQIORISC-V

  In Arduino IDE 2 select **Esp32c6 Dev** from the esp32 by Espressif package. In PlatformIO use `board = esp32-c6-devkitc-1`. ESP32C6 · 160 MHz · 4MB · QIO · RISC-V.

  Arduino IDEPlatformIOESPHomeesptool

  Tools menu settingsCopy

  Board (select in Arduino IDE)Esp32c6 Dev

  CPU Frequency160 MHz

  Flash Size4MB

  Flash ModeQIO

  Upload Speed921600

  ArchitectureRISC-V

  In Arduino IDE 2, open *Boards Manager*, search **"esp32"** by Espressif and install it. Then go to *Tools → Board* and select **"Esp32c6 Dev"** for the ESP32-C6 Super Mini.

  ## ESP32-C6 Super Mini Pinout Diagram

  Complete pin reference for ESP32-C6 Super Mini

  [![ESP32-C6 Super Mini pinout diagram](https://www.espboards.dev/img/uGIogYTad2-1000.png)](https://www.espboards.dev/img/uGIogYTad2-1000.png)

  The **ESP32-C6 Super Mini pinout** is designed for versatile connectivity in a compact format. The board provides essential power pins such as `5V`, `3.3V`, and `GND` for stable power delivery.

  It features dedicated communication interfaces including `RX` and `TX` for UART, `SDA` and `SCL` for I2C, and `MISO`, `MOSI`, `SCK`, and `SS` for SPI, ensuring seamless integration with peripherals.

  For analog input, the **ESP32-C6 Super Mini** offers ADC pins labeled `A0` to `A5`, supporting sensor applications. Additionally, it features a stamp hole design for direct soldering, providing improved reliability for embedded applications.

###  Safe Pins to Use

These pins are safe for general GPIO usage without boot or system conflicts

IO0

IO1

IO2

IO3

IO14

IO20

IO21

IO22

IO23

#### Why Are These Pins Safe?

✓No boot sequence involvement

✓No flash/PSRAM connections

✓No USB or JTAG conflicts

✓Freely assignable without issues

### Pins to Avoid or Use with Caution

Reserved for critical functions. Misuse may cause boot failures, programming issues, or system conflicts.

Strapping Pins

Boot behavior & flash voltage

JTAG Debugging

Low-level debugging interface

USB Pins

USB Serial/JTAG communication

Flash/SPI Pins

Memory & PSRAM connections

UART Serial

Debugging & firmware uploads

| PIN     | Label | Why Avoid                                                    | Type        |
| ------- | ----- | ------------------------------------------------------------ | ----------- |
| **IO4** | MTMS  | Used during boot; required for JTAG debugging; flash data in internal-flash models. | 🛠️ Strapping |
| **IO5** | MTDI  | Used during boot; required for JTAG debugging; flash data in internal-flash models. | 🛠️ Strapping |
| **IO6** | MTCK  | Required for JTAG debugging; connected to flash clock in internal-flash models. | 🔗 JTAG      |
| **IO7** | MTDO  | Required for JTAG debugging; connected to flash data in internal-flash models. | 🔗 JTAG      |
| **IO8** | GPIO8 | Determines boot mode; pulling low at reset can prevent normal boot. | 🛠️ Strapping |

Show All 11 Pins

## On-Board LEDs

LED indicators on ESP32-C6 Super Mini

![ESP32-C6 Super Mini on board LEDs](https://www.espboards.dev/img/nyFRvmgzB4-500.png)

The ESP32-C6 Supermini features three onboard LEDs: a green battery indicator, a user-controllable status LED, and a WS2812 RGB LED. The WS2812 is connected to GPIO8, while the simple status LED is on GPIO15.

### 🟢 Green LED – Battery Charge Indicator

- **GPIO:** `None`
- **Control:** Not controllable via GPIO
- **Behavior:**
  - ⚡ Charging → LED on
  - ✅ Battery connected → LED off
  - 🔋 No battery → LED blinks

### 🔵 Status LED – User Controllable

- **GPIO:** `GPIO15`
- **Control:** `digitalWrite()`, ESPHome GPIO output

**Arduino Example:**

```
void setup() {
  pinMode(15, OUTPUT);
}

void loop() {
  digitalWrite(15, HIGH);
  delay(1000);
  digitalWrite(15, LOW);
  delay(1000);
}
```

**ESPHome Example:**

```
output:
  - platform: gpio
    pin: 15
    id: status_led

light:
  - platform: binary
    name: "Status LED"
    output: status_led
```

[ESPHome GPIO Output Documentation →](https://esphome.io/components/output/gpio.html)

### 🌈 WS2812 LED – Programmable RGB

- **GPIO:** `GPIO8`
- **Control:** FastLED, NeoPixel, etc.

**Arduino (FastLED) Example:**

```
#include <FastLED.h>

#define NUM_LEDS 1
#define DATA_PIN 8

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  leds[0] = CRGB::Red; FastLED.show(); delay(1000);
  leds[0] = CRGB::Green; FastLED.show(); delay(1000);
  leds[0] = CRGB::Blue; FastLED.show(); delay(1000);
```

**ESPHome (ESP32 RMT LED Strip) Example:**

```
light:
  - platform: esp32_rmt_led_strip
    chipset: WS2812
    pin: GPIO8
    num_leds: 1
    rgb_order: GRB
    name: "Onboard RGB LED"
```

[ESPHome ESP32 RMT LED Strip Documentation →](https://esphome.io/components/light/esp32_rmt_led_strip.html)

<iframe src="https://imasdk.googleapis.com/js/core/bridge3.745.0_en.html?gdpr=1#deid=%22%22&amp;eventfe_experiment_ids=%5B%5D&amp;fid=%22goog_263160803%22&amp;genotype_experiment_data=%7B%22experimentStateProto%22%3A%22%5B%5B%5B45713128%2Cnull%2Cnull%2C%5B%5D%5D%2C%5Bnull%2C745150931%2Cnull%2C%5Bnull%2C1%5D%5D%2C%5Bnull%2C749060184%2Cnull%2C%5Bnull%2C128%5D%5D%2C%5B841585769%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45761044%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45722344%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45706017%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45740207%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45668885%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45685340%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45734716%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45735891%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45663239%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45715032%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45661356%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B839547366%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45676441%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45675307%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45675308%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5Bnull%2C45645574%2Cnull%2C%5B%5D%5D%2C%5B45688859%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45656766%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45710689%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45710688%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45685601%2Cnull%2Cnull%2C%5B%5D%5D%2C%5Bnull%2C45685602%2Cnull%2C%5Bnull%2C500%5D%5D%2C%5B45756824%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45747172%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B775241416%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B781107959%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B781107958%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B792614055%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B781107957%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45729602%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45753603%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45753604%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45658982%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45725657%2Cnull%2Cnull%2C%5B%5D%5D%5D%2C%5B%5B16%2C%5B%5B1%2C%5B%5B31089630%5D%2C%5B31089631%2C%5B%5B45668885%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%5D%5D%2C%5B1000%2C%5B%5B95332046%5D%5D%5D%2C%5Bnull%2C%5B%5B95332047%5D%5D%5D%2C%5B10%2C%5B%5B95338769%2C%5B%5Bnull%2C45645574%2Cnull%2C%5Bnull%2C1%5D%5D%5D%5D%2C%5B95338770%2C%5B%5Bnull%2C45645574%2Cnull%2C%5Bnull%2C2%5D%5D%5D%5D%5D%5D%2C%5B50%2C%5B%5B95345206%5D%2C%5B95345207%2C%5B%5B45661356%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%5D%5D%2C%5B1%2C%5B%5B95351425%5D%2C%5B95351426%2C%5B%5B45676441%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%5D%5D%2C%5B20%2C%5B%5B95356068%5D%2C%5B95356069%2C%5B%5B45685601%2Cnull%2Cnull%2C%5B%5D%5D%2C%5Bnull%2C45685602%2Cnull%2C%5B%5D%5D%5D%5D%2C%5B95356070%2C%5B%5B45685601%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5Bnull%2C45685602%2Cnull%2C%5B%5D%5D%5D%5D%2C%5B95356071%2C%5B%5B45685601%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5Bnull%2C45685602%2Cnull%2C%5Bnull%2C100%5D%5D%5D%5D%5D%5D%2C%5B1%2C%5B%5B95373378%2C%5B%5B792614055%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%2C%5B95373379%2C%5B%5B45747172%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B781107959%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B792614055%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B781107957%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%5D%5D%2C%5Bnull%2C%5B%5B95375930%5D%2C%5B95375931%2C%5B%5B45734716%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%2C%5B95376520%2C%5B%5B45734716%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45735891%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%5D%5D%2C%5Bnull%2C%5B%5B95378095%5D%2C%5B95378096%2C%5B%5B45740207%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%5D%5D%2C%5B10%2C%5B%5B95378629%5D%2C%5B95378630%2C%5B%5B45729602%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%2C%5B95381582%2C%5B%5B45729602%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45753603%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%2C%5B95381583%2C%5B%5B45729602%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45753604%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%5D%5D%2C%5Bnull%2C%5B%5B95382403%5D%2C%5B95382404%2C%5B%5B45756824%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%5D%5D%5D%5D%5D%2Cnull%2Cnull%2C%5Bnull%2C1000%2C1%2C1000%5D%5D%22%7D&amp;imalib_experiments=%5B44752711%2C95322027%2C95331589%2C95332046%2C31061690%5D&amp;is_eap_loader=false&amp;managed_js_experiment_id=0&amp;page_correlator=1689467922938308&amp;pvsid=3861992213309040&amp;top_accessible_page_url=%22https%3A%2F%2Fwww.espboards.dev%2Fesp32%2Fesp32-c6-super-mini%2F%22" allowfullscreen="" allow="autoplay;attribution-reporting" id="goog_263160803" title="Advertisement" width="444" height="250" style="box-sizing: border-box; border: 0px; margin: 0px; padding: 0px; vertical-align: middle; display: block; opacity: 1; color-scheme: light;"></iframe>

<iframe title="Advertisement" style="box-sizing: border-box; border: 0px solid; margin: 0px; padding: 0px; vertical-align: middle; display: block; visibility: hidden; width: 0px; height: 0px; left: 0px; top: 0px;"></iframe>

## Useful Links

Datasheets and resources for ESP32-C6 Super Mini



Board Datasheet

Technical specifications



ESP32C6 Datasheet

Microcontroller reference

## ESP32-C6 Super Mini Custom Pin Mapping

Pin configuration and GPIO mapping for ESP32-C6 Super Mini

11

Digital I/O Pins

22

Interrupt Pins

6

Analog Inputs

11

PWM Pins

| Pin    | Function | ESP Pin | I/O Type      | Description                       |
| ------ | -------- | ------- | ------------- | --------------------------------- |
| **1**  | 5V       | 5V      | POWER INPUT   | 5V power input for the board      |
| **2**  | GND      | GND     | POWER GROUND  | Ground connection                 |
| **3**  | 3V3      | 3.3V    | POWER OUTPUT  | 3.3V power output for peripherals |
| **4**  | TX       | TX      | TX            | TX                                |
| **5**  | RX       | RX      | RX            | RX                                |
| **6**  | IO0      | GP0     | BIDIRECTIONAL | GPIO, ADC pin                     |
| **7**  | IO1      | GP1     | BIDIRECTIONAL | GPIO, ADC pin                     |
| **8**  | IO2      | GP2     | BIDIRECTIONAL | GPIO, ADC pin                     |
| **9**  | IO3      | GP3     | BIDIRECTIONAL | GPIO, ADC pin                     |
| **10** | IO4      | GP4     | BIDIRECTIONAL | GPIO, ADC pin                     |
| **11** | IO5      | GP5     | BIDIRECTIONAL | GPIO, ADC pin                     |
| **12** | IO6      | GP6     | BIDIRECTIONAL | GPIO, ADC pin                     |
| **13** | IO7      | GP7     | BIDIRECTIONAL | GPIO                              |
| **14** | IO8      | GP8     | BIDIRECTIONAL | GPIO, RGB LED                     |
| **15** | IO9      | GP9     | BIDIRECTIONAL | GPIO, Boot                        |
| **16** | IO12     | GP12    | BIDIRECTIONAL | GPIO                              |
| **17** | IO13     | GP13    | BIDIRECTIONAL | GPIO                              |
| **18** | IO14     | GP14    | BIDIRECTIONAL | GPIO                              |
| **19** | IO15     | GP15    | BIDIRECTIONAL | GPIO, LED                         |
| **20** | IO18     | GP18    | BIDIRECTIONAL | GPIO                              |
| **21** | IO19     | GP19    | BIDIRECTIONAL | GPIO                              |
| **22** | IO20     | GP20    | BIDIRECTIONAL | GPIO                              |
| **23** | IO21     | GP21    | BIDIRECTIONAL | GPIO                              |
| **24** | IO22     | GP22    | BIDIRECTIONAL | GPIO                              |
| **25** | IO23     | GP23    | BIDIRECTIONAL | GPIO                              |

Legend

FunctionPin role

GPIOESP32 pin

I/ODirection

**#**Pin number

<iframe src="https://imasdk.googleapis.com/js/core/bridge3.745.0_en.html?gdpr=1#deid=%22%22&amp;eventfe_experiment_ids=%5B%5D&amp;fid=%22goog_263160805%22&amp;genotype_experiment_data=%7B%22experimentStateProto%22%3A%22%5B%5B%5B45713128%2Cnull%2Cnull%2C%5B%5D%5D%2C%5Bnull%2C745150931%2Cnull%2C%5Bnull%2C1%5D%5D%2C%5Bnull%2C749060184%2Cnull%2C%5Bnull%2C128%5D%5D%2C%5B841585769%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45761044%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45722344%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45706017%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45740207%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45668885%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45685340%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45734716%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45735891%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45663239%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45715032%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45661356%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B839547366%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45676441%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45675307%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45675308%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5Bnull%2C45645574%2Cnull%2C%5B%5D%5D%2C%5B45688859%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45656766%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45710689%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45710688%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45685601%2Cnull%2Cnull%2C%5B%5D%5D%2C%5Bnull%2C45685602%2Cnull%2C%5Bnull%2C500%5D%5D%2C%5B45756824%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45747172%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B775241416%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B781107959%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B781107958%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B792614055%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B781107957%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45729602%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45753603%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45753604%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45658982%2Cnull%2Cnull%2C%5B%5D%5D%2C%5B45725657%2Cnull%2Cnull%2C%5B%5D%5D%5D%2C%5B%5B16%2C%5B%5B1%2C%5B%5B31089630%5D%2C%5B31089631%2C%5B%5B45668885%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%5D%5D%2C%5B1000%2C%5B%5B95332046%5D%5D%5D%2C%5Bnull%2C%5B%5B95332047%5D%5D%5D%2C%5B10%2C%5B%5B95338769%2C%5B%5Bnull%2C45645574%2Cnull%2C%5Bnull%2C1%5D%5D%5D%5D%2C%5B95338770%2C%5B%5Bnull%2C45645574%2Cnull%2C%5Bnull%2C2%5D%5D%5D%5D%5D%5D%2C%5B50%2C%5B%5B95345206%5D%2C%5B95345207%2C%5B%5B45661356%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%5D%5D%2C%5B1%2C%5B%5B95351425%5D%2C%5B95351426%2C%5B%5B45676441%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%5D%5D%2C%5B20%2C%5B%5B95356068%5D%2C%5B95356069%2C%5B%5B45685601%2Cnull%2Cnull%2C%5B%5D%5D%2C%5Bnull%2C45685602%2Cnull%2C%5B%5D%5D%5D%5D%2C%5B95356070%2C%5B%5B45685601%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5Bnull%2C45685602%2Cnull%2C%5B%5D%5D%5D%5D%2C%5B95356071%2C%5B%5B45685601%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5Bnull%2C45685602%2Cnull%2C%5Bnull%2C100%5D%5D%5D%5D%5D%5D%2C%5B1%2C%5B%5B95373378%2C%5B%5B792614055%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%2C%5B95373379%2C%5B%5B45747172%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B781107959%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B792614055%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B781107957%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%5D%5D%2C%5Bnull%2C%5B%5B95375930%5D%2C%5B95375931%2C%5B%5B45734716%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%2C%5B95376520%2C%5B%5B45734716%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45735891%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%5D%5D%2C%5Bnull%2C%5B%5B95378095%5D%2C%5B95378096%2C%5B%5B45740207%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%5D%5D%2C%5B10%2C%5B%5B95378629%5D%2C%5B95378630%2C%5B%5B45729602%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%2C%5B95381582%2C%5B%5B45729602%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45753603%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%2C%5B95381583%2C%5B%5B45729602%2Cnull%2Cnull%2C%5B1%5D%5D%2C%5B45753604%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%5D%5D%2C%5Bnull%2C%5B%5B95382403%5D%2C%5B95382404%2C%5B%5B45756824%2Cnull%2Cnull%2C%5B1%5D%5D%5D%5D%5D%5D%5D%5D%5D%2Cnull%2Cnull%2C%5Bnull%2C1000%2C1%2C1000%5D%5D%22%7D&amp;imalib_experiments=%5B44752711%2C95322027%2C95331589%2C95332046%2C31061690%5D&amp;is_eap_loader=false&amp;managed_js_experiment_id=0&amp;page_correlator=1172156510275213&amp;pvsid=3861992213309040&amp;top_accessible_page_url=%22https%3A%2F%2Fwww.espboards.dev%2Fesp32%2Fesp32-c6-super-mini%2F%22" allowfullscreen="" allow="autoplay;attribution-reporting" id="goog_263160805" title="Advertisement" width="444" height="250" style="box-sizing: border-box; border: 0px; margin: 0px; padding: 0px; vertical-align: middle; display: block; opacity: 1; color-scheme: light;"></iframe>

<iframe title="Advertisement" style="box-sizing: border-box; border: 0px solid; margin: 0px; padding: 0px; vertical-align: middle; display: block; visibility: hidden; width: 0px; height: 0px; left: 0px; top: 0px;"></iframe>

<iframe frameborder="0" src="https://6cde1e250af0d43b8d5e01dd955bec5e.safeframe.googlesyndication.com/safeframe/1-0-45/html/container.html" id="google_ads_iframe_/22794612459,23334967815/espboards/content_4" title="3rd party ad content" name="" scrolling="no" marginwidth="0" marginheight="0" width="728" height="280" data-is-safeframe="true" sandbox="allow-forms allow-popups allow-popups-to-escape-sandbox allow-same-origin allow-scripts allow-top-navigation-by-user-activation" allow="private-state-token-redemption;attribution-reporting" aria-label="Advertisement" tabindex="0" data-google-container-id="8" data-load-complete="true" data-hooks="true" style="box-sizing: border-box; border: 0px; margin: 0px auto !important; padding: 0px; vertical-align: bottom; display: block; text-align: center; height: 280px; width: 728px;"></iframe>

![img](data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8+CjwhLS0gQ3JlYXRlZCB3aXRoIElua3NjYXBlIChodHRwOi8vd3d3Lmlua3NjYXBlLm9yZy8pIC0tPgoKPHN2ZwogICB2ZXJzaW9uPSIxLjEiCiAgIGlkPSJzdmcyIgogICB3aWR0aD0iMjYuNjY2NjY4IgogICBoZWlnaHQ9IjI2LjY2NjY2OCIKICAgdmlld0JveD0iMCAwIDI2LjY2NjY2OCAyNi42NjY2NjgiCiAgIHNvZGlwb2RpOmRvY25hbWU9IlB1Yk5hdGlvbi1SZXBvcnRpbmctSWNvbi5haSIKICAgeG1sbnM6aW5rc2NhcGU9Imh0dHA6Ly93d3cuaW5rc2NhcGUub3JnL25hbWVzcGFjZXMvaW5rc2NhcGUiCiAgIHhtbG5zOnNvZGlwb2RpPSJodHRwOi8vc29kaXBvZGkuc291cmNlZm9yZ2UubmV0L0RURC9zb2RpcG9kaS0wLmR0ZCIKICAgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIgogICB4bWxuczpzdmc9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KICA8ZGVmcwogICAgIGlkPSJkZWZzNiI+CiAgICA8Y2xpcFBhdGgKICAgICAgIGNsaXBQYXRoVW5pdHM9InVzZXJTcGFjZU9uVXNlIgogICAgICAgaWQ9ImNsaXBQYXRoMTYiPgogICAgICA8cGF0aAogICAgICAgICBkPSJNIDAsMjAgSCAyMCBWIDAgSCAwIFoiCiAgICAgICAgIGlkPSJwYXRoMTQiIC8+CiAgICA8L2NsaXBQYXRoPgogIDwvZGVmcz4KICA8c29kaXBvZGk6bmFtZWR2aWV3CiAgICAgaWQ9Im5hbWVkdmlldzQiCiAgICAgcGFnZWNvbG9yPSIjZmZmZmZmIgogICAgIGJvcmRlcmNvbG9yPSIjMDAwMDAwIgogICAgIGJvcmRlcm9wYWNpdHk9IjAuMjUiCiAgICAgaW5rc2NhcGU6c2hvd3BhZ2VzaGFkb3c9IjIiCiAgICAgaW5rc2NhcGU6cGFnZW9wYWNpdHk9IjAuMCIKICAgICBpbmtzY2FwZTpwYWdlY2hlY2tlcmJvYXJkPSIwIgogICAgIGlua3NjYXBlOmRlc2tjb2xvcj0iI2QxZDFkMSIgLz4KICA8ZwogICAgIGlkPSJnOCIKICAgICBpbmtzY2FwZTpncm91cG1vZGU9ImxheWVyIgogICAgIGlua3NjYXBlOmxhYmVsPSJQdWJOYXRpb24tUmVwb3J0aW5nLUljb24iCiAgICAgdHJhbnNmb3JtPSJtYXRyaXgoMS4zMzMzMzMzLDAsMCwtMS4zMzMzMzMzLDAsMjYuNjY2NjY3KSI+CiAgICA8ZwogICAgICAgaWQ9ImcxMCI+CiAgICAgIDxnCiAgICAgICAgIGlkPSJnMTIiCiAgICAgICAgIGNsaXAtcGF0aD0idXJsKCNjbGlwUGF0aDE2KSI+CiAgICAgICAgPGcKICAgICAgICAgICBpZD0iZzE4IgogICAgICAgICAgIHRyYW5zZm9ybT0idHJhbnNsYXRlKDUuMDEwNCw5LjUzMjgpIj4KICAgICAgICAgIDxwYXRoCiAgICAgICAgICAgICBkPSJtIDAsMCBjIDEuMDQsMCAxLjYxMywwLjU1OCAxLjYxMywxLjcxOCAwLDEuMTMgLTAuNjMzLDEuNzYzIC0xLjgwOSwxLjc2MyBIIC0xLjkxNCBWIDAgWiBtIC00LjM4NSw1LjQyNSBoIDQuMjY0IGMgMi41MzIsMCA0LjA1NCwtMS4xOSA0LjA1NCwtMy41ODYgMCwtMi4yMDEgLTEuMjM1LC0zLjQ5NyAtMy44MjcsLTMuNDk3IGggLTIuMDIgdiAtMi44MzMgaCAtMi40NzEgeiIKICAgICAgICAgICAgIHN0eWxlPSJmaWxsOiM5MTkxOTE7ZmlsbC1vcGFjaXR5OjE7ZmlsbC1ydWxlOm5vbnplcm87c3Ryb2tlOm5vbmUiCiAgICAgICAgICAgICBpZD0icGF0aDIwIiAvPgogICAgICAgIDwvZz4KICAgICAgICA8ZwogICAgICAgICAgIGlkPSJnMjIiCiAgICAgICAgICAgdHJhbnNmb3JtPSJ0cmFuc2xhdGUoMTAuNzI0NSwxNC45NTgpIj4KICAgICAgICAgIDxwYXRoCiAgICAgICAgICAgICBkPSJtIDAsMCBoIDIuMDM0IGwgNS4zODEsLTguMjI4IGggMC4wMyBWIDAgSCA4LjY1IFYgLTkuOTE2IEggNy4xNTggTCAxLjIzNiwtMC45MTkgSCAxLjIwNiBWIC05LjkxNiBIIDAgWiIKICAgICAgICAgICAgIHN0eWxlPSJmaWxsOiM5MTkxOTE7ZmlsbC1vcGFjaXR5OjE7ZmlsbC1ydWxlOm5vbnplcm87c3Ryb2tlOm5vbmUiCiAgICAgICAgICAgICBpZD0icGF0aDI0IiAvPgogICAgICAgIDwvZz4KICAgICAgPC9nPgogICAgPC9nPgogIDwvZz4KPC9zdmc+Cg==)

## Pin Mappings

Complete pinout and GPIO mapping for ESP32-C6 Super Mini

11

Digital I/O Pins

22

Interrupt Pins

6

Analog Inputs

11

PWM Pins

| Pin    | Analog | Touch | PWM  | Other |
| ------ | ------ | ----- | ---- | ----- |
| **0**  | A0     |       |      |       |
| **1**  | A1     |       |      |       |
| **2**  | A2     |       |      |       |
| **3**  | A3     |       |      |       |
| **4**  | A4     |       |      | SCK   |
| **5**  | A5     |       |      | MISO  |
| **6**  |        |       |      | MOSI  |
| **7**  |        |       |      | SS    |
| **8**  |        |       |      | SDA   |
| **9**  |        |       |      | SCL   |
| **20** |        |       |      | RX    |
| **21** |        |       |      | TX    |

Legend

A0Analog input

T0Touch sensor

PWMPWM capable

RX/TXSerial pins

LEDBuilt-in LED

**#**Pin number

## Default Tools & Configuration

Build and upload settings for ESP32-C6 Super Mini

| Setting               | Value                      |
| --------------------- | -------------------------- |
| Bootloader tool       | esptool_py                 |
| Uploader tool         | esptool_py                 |
| Network uploader tool | esp_ota                    |
| Bootloader address    | `0x0`                      |
| Flash mode            | qio                        |
| Boot mode             | qio                        |
| Maximum upload size   | **1280 KB**(1310720 bytes) |
| Maximum data size     | **320 KB**(327680 bytes)   |

Configuration Summary

The **ESP32-C6 Super Mini** uses **esptool_py** for uploads , **esp_ota** for OTA updates, and **esptool_py** bootloader at `0x0`.

Flash mode: **qio** | Boot mode: **qio**

Max sketch size: **1280 KB** | Max data size: **320 KB**