#include <WiFi.h>
#include <PubSubClient.h>
#include <Preferences.h>
#include <Adafruit_NeoPixel.h>

//
// ============================================================
// ===================== CONFIGURATION ========================
// ============================================================
//

// -------- WLAN ----------
static const char* WIFI_SSID = "RouterJoachimWillner";
static const char* WIFI_PASS = "JoWiBu-456";

// -------- MQTT ----------
static const char* MQTT_HOST = "192.168.1.16";
static const uint16_t MQTT_PORT = 1883;

static const char* MQTT_CLIENT_ID   = "gas_counter_esp32c6";
static const char* MQTT_TOPIC_STATE = "gas_counter/state";
static const char* MQTT_TOPIC_GPIO  = "gas_counter/gpio3";
static const char* MQTT_TOPIC_AVAIL = "gas_counter/availability";
static const char* MQTT_TOPIC_SUB   = "gas_counter/cmd/#";   // Debug RX

// -------- Hardware ----------
static const uint8_t PIN_SENSOR   = 3;        // GPIO3 Sensor input
static const uint8_t PIN_NEOPIXEL = 8;        // WS2812 data pin (typisch ESP32-C6 Zero/SuperMini)
static const uint8_t NUM_PIXELS   = 1;

// -------- Gas parameters ----------
static const float    PULSE_VOLUME_M3 = 0.01f;   // 1 Impuls = 0.01 m³
static const float    GAS_KWH_PER_M3  = 10.5f;   // ggf. anpassen!
static const uint32_t DEBOUNCE_US     = 20000UL; // 20ms

// -------- Timing ----------
static const uint32_t WIFI_TIMEOUT_MS         = 20000UL;
static const uint32_t WIFI_RETRY_DELAY_MS     = 300UL;
static const uint32_t MQTT_RETRY_DELAY_MS     = 2000UL;

static const uint32_t GPIO_POLL_INTERVAL_MS   = 20UL;       // nur für LED/GPIO state publish
static const uint32_t PUBLISH_INTERVAL_MS     = 10000UL;    // regelmäßiges publish
static const uint32_t FORCE_PUBLISH_MIN_GAP_MS= 250UL;      // Anti-spam bei Puls-Serien

static const uint32_t PERSIST_INTERVAL_MS     = 60000UL;    // total speichern
static const uint32_t HOUR_INTERVAL_MS        = 3600000UL;  // Stunde ab Boot
static const uint32_t LOOP_DELAY_MS           = 5UL;

// -------- LED ----------
static const uint8_t LED_BRIGHTNESS = 40;

static const uint8_t LED_GREEN_R = 0;
static const uint8_t LED_GREEN_G = 150;
static const uint8_t LED_GREEN_B = 0;

static const uint8_t LED_RED_R   = 150;
static const uint8_t LED_RED_G   = 0;
static const uint8_t LED_RED_B   = 0;

static const uint8_t LED_OFF_R   = 0;
static const uint8_t LED_OFF_G   = 0;
static const uint8_t LED_OFF_B   = 0;

// -------- NVS ----------
static const char* NVS_NAMESPACE     = "gas";
static const char* NVS_KEY_TOTAL_MWH = "total_mWh"; // total_kwh als mWh (kWh*1,000,000)

//
// ============================================================
// ===================== GLOBALS ==============================
// ============================================================
//

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
Preferences prefs;

Adafruit_NeoPixel strip(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// ISR counters
volatile uint32_t pulses_total = 0;
volatile uint32_t pulses_hour  = 0;
volatile uint32_t pending_pulses = 0;
volatile uint32_t last_isr_us = 0;

uint32_t hourStartMs = 0;
float total_kwh_offset = 0.0f;

int last_gpio_level = HIGH;
uint32_t lastGpioPollMs = 0;

uint32_t lastPublishMs = 0;
uint32_t lastForcePublishMs = 0;

uint32_t lastPersistMs = 0;

//
// ============================================================
// ===================== UTIL =================================
// ============================================================
//

static inline const char* levelToStr(int level) {
  return (level == LOW) ? "LOW" : "HIGH";
}

static inline float pulsesToKwh(uint32_t p) {
  return (p * PULSE_VOLUME_M3) * GAS_KWH_PER_M3;
}

// store kWh as mWh (uint32)
static inline uint32_t kWhToMWh(float kwh) {
  if (kwh <= 0.0f) return 0U;
  double v = (double)kwh * 1000000.0;
  if (v > 4294967295.0) v = 4294967295.0;
  return (uint32_t)(v + 0.5);
}
static inline float mWhToKWh(uint32_t mwh) {
  return (float)mwh / 1000000.0f;
}

static inline void setLed(uint8_t r, uint8_t g, uint8_t b) {
  strip.setPixelColor(0, strip.Color(r, g, b));
  strip.show();
}

static inline void applyLedFromLevel(int level) {
  if (level == LOW) {
    setLed(LED_GREEN_R, LED_GREEN_G, LED_GREEN_B);
  } else {
    setLed(LED_RED_R, LED_RED_G, LED_RED_B);
  }
}

//
// ============================================================
// ===================== ISR ==================================
// ============================================================
//

void IRAM_ATTR isrPulse() {
  uint32_t now = (uint32_t)micros();
  if ((uint32_t)(now - last_isr_us) < DEBOUNCE_US) return;
  last_isr_us = now;

  pulses_total++;
  pulses_hour++;
  pending_pulses++;
}

//
// ============================================================
// ===================== MQTT RX ==============================
// ============================================================
//

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  static const uint16_t MAX_PRINT = 512;
  uint16_t n = (length > MAX_PRINT) ? MAX_PRINT : (uint16_t)length;

  Serial.print("MQTT RX topic: ");
  Serial.print(topic);
  Serial.print("\r\nMQTT RX payload: ");

  for (uint16_t i = 0; i < n; i++) Serial.print((char)payload[i]);
  if (length > MAX_PRINT) Serial.print("...(truncated)");

  Serial.print("\r\n\r\n");
}

//
// ============================================================
// ===================== WIFI =================================
// ============================================================
//

void wifiConnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("WiFi connecting...\r\n");

  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_RETRY_DELAY_MS);
    if ((uint32_t)(millis() - start) > WIFI_TIMEOUT_MS) {
      Serial.print("WiFi timeout -> reboot\r\n");
      delay(500);
      ESP.restart();
    }
  }

  Serial.print("WiFi connected, IP=");
  Serial.print(WiFi.localIP());
  Serial.print("\r\n");
}

//
// ============================================================
// ===================== MQTT =================================
// ============================================================
//

void mqttConnect() {
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(mqttCallback);

  while (!mqtt.connected()) {
    Serial.print("MQTT connecting to ");
    Serial.print(MQTT_HOST);
    Serial.print(":");
    Serial.print(MQTT_PORT);
    Serial.print("\r\n");

    bool ok = mqtt.connect(MQTT_CLIENT_ID, MQTT_TOPIC_AVAIL, 1, true, "offline");
    if (ok) {
      Serial.print("MQTT connected\r\n");

      // availability online (retained)
      mqtt.publish(MQTT_TOPIC_AVAIL, "online", true);
      Serial.print("MQTT TX topic: ");
      Serial.print(MQTT_TOPIC_AVAIL);
      Serial.print("\r\nMQTT TX payload: online\r\n\r\n");

      // debug subscribe
      mqtt.subscribe(MQTT_TOPIC_SUB);
      Serial.print("MQTT SUB: ");
      Serial.print(MQTT_TOPIC_SUB);
      Serial.print("\r\n\r\n");

      // publish current gpio level retained (so HA/MQTT sees initial state)
      int level = digitalRead(PIN_SENSOR);
      mqtt.publish(MQTT_TOPIC_GPIO, levelToStr(level), true);
      Serial.print("MQTT TX topic: ");
      Serial.print(MQTT_TOPIC_GPIO);
      Serial.print("\r\nMQTT TX payload: ");
      Serial.print(levelToStr(level));
      Serial.print("\r\n\r\n");
    } else {
      Serial.print("MQTT connect failed rc=");
      Serial.print(mqtt.state());
      Serial.print(" -> retry\r\n");
      delay(MQTT_RETRY_DELAY_MS);
    }
  }
}

//
// ============================================================
// ===================== PUBLISH ==============================
// ============================================================
//

void publishAll(bool force) {
  uint32_t now = millis();

  if (force) {
    if ((uint32_t)(now - lastForcePublishMs) < FORCE_PUBLISH_MIN_GAP_MS) return;
    lastForcePublishMs = now;
  } else {
    if ((uint32_t)(now - lastPublishMs) < PUBLISH_INTERVAL_MS) return;
  }

  lastPublishMs = now;

  uint32_t t, h;
  noInterrupts();
  t = pulses_total;
  h = pulses_hour;
  interrupts();

  int level = digitalRead(PIN_SENSOR);

  float total_kwh = total_kwh_offset + pulsesToKwh(t);
  float hour_kwh  = pulsesToKwh(h);

  // --- GPIO retained topic ---
  if (mqtt.connected()) {
    bool ok1 = mqtt.publish(MQTT_TOPIC_GPIO, levelToStr(level), true);
    Serial.print("MQTT TX topic: ");
    Serial.print(MQTT_TOPIC_GPIO);
    Serial.print("\r\nMQTT TX payload: ");
    Serial.print(levelToStr(level));
    Serial.print("\r\nMQTT TX result: ");
    Serial.print(ok1 ? "OK" : "FAILED");
    Serial.print("\r\n\r\n");
  }

  // --- JSON state ---
  char payload[256];
  snprintf(payload, sizeof(payload),
           "{\"gpio3\":\"%s\",\"total_kwh\":%.3f,\"hour_kwh\":%.3f,"
           "\"pulses_total\":%u,\"pulses_hour\":%u,\"rssi\":%d}",
           levelToStr(level),
           total_kwh, hour_kwh,
           t, h, WiFi.RSSI());

  if (mqtt.connected()) {
    bool ok2 = mqtt.publish(MQTT_TOPIC_STATE, payload, true);
    Serial.print("MQTT TX topic: ");
    Serial.print(MQTT_TOPIC_STATE);
    Serial.print("\r\nMQTT TX payload: ");
    Serial.print(payload);
    Serial.print("\r\nMQTT TX result: ");
    Serial.print(ok2 ? "OK" : "FAILED");
    Serial.print("\r\n\r\n");
  }

  // --- persist total regularly ---
  if ((uint32_t)(now - lastPersistMs) >= PERSIST_INTERVAL_MS) {
    lastPersistMs = now;
    prefs.begin(NVS_NAMESPACE, false);
    prefs.putUInt(NVS_KEY_TOTAL_MWH, kWhToMWh(total_kwh));
    prefs.end();
  }
}

//
// ============================================================
// ===================== SETUP/LOOP ===========================
// ============================================================
//

void setup() {
  Serial.begin(115200);
  delay(1500);
  Serial.print("\r\n=== GasCounter GPIO3 + WS2812 + WiFi/MQTT ===\r\n");

  pinMode(PIN_SENSOR, INPUT_PULLUP);

  // WS2812 init
  strip.begin();
  strip.setBrightness(LED_BRIGHTNESS);
  setLed(LED_OFF_R, LED_OFF_G, LED_OFF_B);

  // Load saved total
  prefs.begin(NVS_NAMESPACE, false);
  total_kwh_offset = mWhToKWh(prefs.getUInt(NVS_KEY_TOTAL_MWH, 0));
  prefs.end();

  hourStartMs = millis();

  // Initial LED + state
  last_gpio_level = digitalRead(PIN_SENSOR);
  Serial.print("Initial GPIO3: ");
  Serial.print(levelToStr(last_gpio_level));
  Serial.print("\r\n");
  applyLedFromLevel(last_gpio_level);

  // Pulse interrupt (gültig nach debounce)
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR), isrPulse, FALLING);

  // Connect
  wifiConnect();
  mqttConnect();

  // Initial publish
  publishAll(true);
}

void loop() {
  // Keep WiFi/MQTT alive
  if (WiFi.status() != WL_CONNECTED) {
    wifiConnect();
  }
  if (!mqtt.connected()) {
    mqttConnect();
  }
  mqtt.loop();

  // GPIO poll (nur für LED + change publish/log)
  uint32_t now = millis();
  if ((uint32_t)(now - lastGpioPollMs) >= GPIO_POLL_INTERVAL_MS) {
    lastGpioPollMs = now;

    int level = digitalRead(PIN_SENSOR);
    if (level != last_gpio_level) {
      last_gpio_level = level;

      Serial.print("GPIO3 changed -> ");
      Serial.print(levelToStr(level));
      Serial.print("\r\n");

      applyLedFromLevel(level);

      // retained gpio + state
      publishAll(true);
    }
  }

  // If pulses happened: publish (force, but rate limited)
  if (pending_pulses) {
    noInterrupts();
    pending_pulses = 0;
    interrupts();
    publishAll(true);
  }

  // Periodic publish
  publishAll(false);

  // Hour rollover (A: hour since boot)
  if ((uint32_t)(now - hourStartMs) >= HOUR_INTERVAL_MS) {
    hourStartMs += HOUR_INTERVAL_MS;
    noInterrupts();
    pulses_hour = 0;
    interrupts();

    Serial.print("Hour rollover -> reset hour pulses\r\n");
    publishAll(true);
  }

  delay(LOOP_DELAY_MS);
}
