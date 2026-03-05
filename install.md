# ESP32-C6 Zigbee Gas Counter -- Installation Guide

## Projektverzeichnis

\~/arduino_projects/Zigbee_GasCounter

------------------------------------------------------------------------

# 1. Arduino CLI prüfen

## Installation prüfen

which arduino-cli arduino-cli version

Erwartung:

-   Pfad z. B. `/usr/local/bin/arduino-cli`
-   Version z. B. `1.4.1`

------------------------------------------------------------------------

# 2. ESP32 Core prüfen

arduino-cli core list

Erwartung:

esp32:esp32 3.3.7 3.3.7 esp32

Falls nicht installiert:

arduino-cli config init arduino-cli config set
board_manager.additional_urls
https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli core update-index arduino-cli core install esp32:esp32

------------------------------------------------------------------------

# 3. Board erkennen

Board anschließen und prüfen:

arduino-cli board list

Beispielausgabe:

/dev/ttyACM0 serial Serial Port (USB) ESP32 Family Device

Alle C6 Boards anzeigen:

arduino-cli board listall \| grep -i c6

Für Waveshare ESP32-C6 Zero ist die FQBN:

esp32:esp32:waveshare_esp32_c6_zero

------------------------------------------------------------------------

# 4. Minimal-Testsketch erstellen

cat \> Zigbee_GasCounter.ino \<\<'INO' void setup() {
Serial.begin(115200); delay(500); Serial.println();
Serial.println("ESP32-C6 Zero OK"); }

void loop() { Serial.println("running..."); delay(2000); } INO

------------------------------------------------------------------------

# 5. Kompilieren

arduino-cli compile --fqbn esp32:esp32:waveshare_esp32_c6_zero .

------------------------------------------------------------------------

# 6. Upload

arduino-cli upload\
--fqbn esp32:esp32:waveshare_esp32_c6_zero\
-p /dev/ttyACM0 .

Falls Upload nicht startet:

1.  BOOT gedrückt halten
2.  RESET kurz drücken
3.  BOOT loslassen

------------------------------------------------------------------------

# 7. Serieller Monitor

## Mit arduino-cli

arduino-cli monitor -p /dev/ttyACM0 -c baudrate=115200

## Mit tio

tio /dev/ttyACM0 -b 115200

Beenden mit:

Ctrl + T Q

Falls tio nicht installiert ist:

sudo apt install tio

------------------------------------------------------------------------

# Status

✔ Arduino CLI installiert ✔ ESP32 Core 3.3.7 installiert ✔ Waveshare
ESP32-C6 Zero erkannt ✔ Upload & Serial-Test möglich

------------------------------------------------------------------------

Nächster Schritt: Implementierung des Zigbee Gaszählers als End Device.
