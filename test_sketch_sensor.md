# Test-Sensor-Sketch — GPIO2 Debug

Dieser Test-Sketch hilft dir zu prüfen, ob dein digitaler Eingang (GPIO2) korrekt funktioniert.  
Er liest den Status des Pins und gibt ihn über die serielle Schnittstelle aus.

## Ziel

Mit diesem Sketch kannst du:

- testen, ob ein Pin korrekt als digitaler Eingang arbeitet  
- sehen, wie der Pin auf einen Taster oder Sensor-Kontakt reagiert  
- schnell feststellen, ob die Hardware-Verdrahtung passt

## Hardware

- ESP32-C6 Zero Board  
- Verbindung: GPIO2 → Taster/Sensor → GND  
- Optional: externer Pull-Up-Widerstand 10 kΩ zwischen 3.3 V und GPIO2

## Code — GPIO2 Input Debug

```cpp
void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("GPIO2 Test Start");

  pinMode(2, INPUT_PULLUP); // internes Pull-Up aktivieren
}

void loop() {
  int level = digitalRead(2); // lese Pin
  Serial.println(level);      // 1 = HIGH (= offen), 0 = LOW (= GND verbunden)
  delay(500);                 // Ausgabe alle 500 ms
}