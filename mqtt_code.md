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
static const char* WIFI_PASS = "JoWiBu-4567";

// -------- MQTT ----------
static const char* MQTT_HOST = "192.168.1.16";
static const uint16_t MQTT_PORT = 1883;

static const char* MQTT_CLIENT_ID   = "gas_counter_esp32c6";
static const char* MQTT_TOPIC_STATE = "gas_counter/state";
static const char* MQTT_TOPIC_GPIO2 = "gas_counter/gpio2";
static const char* MQTT_TOPIC_AVAIL = "gas_counter/availability";

// -------- Hardware ----------
static const uint8_t PIN_SENSOR   = 2;        // Sensor input (pulse)
static const uint8_t PIN_NEOPIXEL = 8;        // WS2812 data pin (Waveshare C6 Zero)
static const uint8_t PIN_BUTTON   = BOOT_PIN; // BOOT button
static const uint8_t NUM_PIXELS   = 1;

// -------- Gas Parameters ----------
static const float PULSE_VOLUME_M3 = 0.01f;  // 1 pulse = 0.01 m³
static const float GAS_KWH_PER_M3  = 10.5f;  // adjust for your gas conversion!
static const uint32_t DEBOUNCE_US  = 20000UL; // 20ms debounce in ISR

// -------- Timing ----------
static const uint32_t WIFI_TIMEOUT_MS         = 20000UL;
static const uint32_t WIFI_RETRY_DELAY_MS     = 300UL;

static const uint32_t MQTT_RETRY_DELAY_MS     = 2000UL;

static const uint32_t PUBLISH_INTERVAL_MS     = 10000UL;  // periodic publish
static const uint32_t PUBLISH_MIN_GAP_MS      = 1000UL;   // min gap between forced publishes
static const uint32_t PERSIST_INTERVAL_MS     = 60000UL;  // save total every 60s

static const uint32_t HOUR_INTERVAL_MS        = 3600000UL; // 1h since boot
static const uint32_t MAIN_LOOP_DELAY_MS      = 10UL;

static const uint32_t RED_PULSE_DURATION_MS   = 1000UL;   // 1s red on valid pulse

// -------- LED Brightness ----------
static const uint8_t LED_BRIGHTNESS = 50;

// -------- LED Colors (RGB) ----------
static const uint8_t LED_GREEN_R = 0;
static const uint8_t LED_GREEN_G = 150;
static const uint8_t LED_GREEN_B = 0;

static const uint8_t LED_RED_R   = 150;
static const uint8_t LED_RED_G   = 0;
static const uint8_t LED_RED_B   = 0;

static const uint8_t LED_OFF_R   = 0;
static const uint8_t LED_OFF_G   = 0;
static const uint8_t LED_OFF_B   = 0;

// -------- Storage ----------
static const char* NVS_NAMESPACE = "gas";
static const char* NVS_KEY_TOTAL_MWH = "total_mWh"; // stored as mWh (kWh*1,000,000)

//
// ============================================================
// ===================== GLOBALS ==============================
// ============================================================
//

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
Preferences prefs;

Adafruit_NeoPixel strip(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// Pulse counters (ISR)
volatile uint32_t pulses_total = 0;
volatile uint32_t pulses_hour  = 0;
volatile uint32_t pending_pulses = 0;
volatile uint32_t last_isr_us = 0;

// "valid pulse happened" flag (set only after debounce acceptance)
volatile bool red_pulse_pending = false;

// LED pulse timing
bool redActive = false;
uint32_t redStartMs = 0;

// hour window
uint32_t hourStartMs = 0;

// persistence offset (kWh)
float total_kwh_offset = 0.0f;

// GPIO2 state tracking (for change messages)
int last_gpio_state = -1;

// connectivity state
bool mqttOnline = false;

//
// ============================================================
// ===================== UTIL =================================
// ============================================================
//

static inline float pulsesToKwh(uint32_t p) {
  return (p * PULSE_VOLUME_M3) * GAS_KWH_PER_M3;
}

// store kWh as mWh in uint32 (1kWh = 1,000,000 mWh)
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

//
// ============================================================
// ===================== ISR ==================================
// ============================================================
//

void IRAM_ATTR isrPulse() {
  uint32_t now = (uint32_t)micros();

  if ((uint32_t)(now - last_isr_us) < DEBOUNCE_US) {
    return;
  }

  last_isr_us = now;

  pulses_total++;
  pulses_hour++;
  pending_pulses++;

  // Only set when debounce accepted => "valid pulse"
  red_pulse_pending = true;
}

//
// ============================================================
// ===================== WIFI =================================
// ============================================================
//

void wifiConnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.printf("WiFi connecting to %s\r\n", WIFI_SSID);

  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_RETRY_DELAY_MS);

    if ((uint32_t)(millis() - start) > WIFI_TIMEOUT_MS) {
      Serial.print("WiFi timeout -> reboot\r\n");
      delay(500);
      ESP.restart();
    }
  }

  Serial.printf("WiFi connected, IP=%s\r\n", WiFi.localIP().toString().c_str());
}

//
// ============================================================
// ===================== MQTT =================================
// ============================================================
//

void mqttConnect() {
  mqtt.setServer(MQTT_HOST, MQTT_PORT);

  while (!mqtt.connected()) {
    mqttOnline = false;

    Serial.printf("MQTT connecting to %s:%u\r\n", MQTT_HOST, MQTT_PORT);
    
    // LWT: publish offline if device drops
    if (mqtt.connect(MQTT_CLIENT_ID, MQTT_TOPIC_AVAIL, 1, true, "offline")) {
      Serial.print("MQTT connected\r\n");
      mqtt.publish(MQTT_TOPIC_AVAIL, "online", true);
      mqttOnline = true;
    
      // publish initial gpio state retained
      int s = digitalRead(PIN_SENSOR);
      mqtt.publish(MQTT_TOPIC_GPIO2, (s == LOW) ? "LOW" : "HIGH", true);
    } else {
      Serial.printf("MQTT connect failed rc=%d -> retry\r\n", mqtt.state());
      delay(MQTT_RETRY_DELAY_MS);
    }
  }
}

//
// ============================================================
// ===================== PUBLISH ==============================
// ============================================================
//

void publishState(bool force) {
  static uint32_t lastPublishMs = 0;
  static uint32_t lastForceMs = 0;

  uint32_t now = millis();

  if (!force) {
    if ((uint32_t)(now - lastPublishMs) < PUBLISH_INTERVAL_MS) {
      return;
    }
  } else {
    if ((uint32_t)(now - lastForceMs) < PUBLISH_MIN_GAP_MS) {
      return;
    }
    lastForceMs = now;
  }

  lastPublishMs = now;

  uint32_t t, h;
  noInterrupts();
  t = pulses_total;
  h = pulses_hour;
  interrupts();

  float total_kwh = total_kwh_offset + pulsesToKwh(t);
  float hour_kwh  = pulsesToKwh(h);

  char payload[256];
  snprintf(payload, sizeof(payload),
           "{\"total_kwh\":%.3f,\"hour_kwh\":%.3f,"
           "\"pulses_total\":%u,\"pulses_hour\":%u,"
           "\"rssi\":%d}",
           total_kwh, hour_kwh, t, h, WiFi.RSSI());

  if (mqtt.connected()) {
    bool ok = mqtt.publish(MQTT_TOPIC_STATE, payload, true);

    Serial.printf("MQTT publish -> %s\r\n", MQTT_TOPIC_STATE);
    Serial.printf("Payload: %s\r\n", payload);
    Serial.printf("Result: %s\r\n\r\n", ok ? "OK" : "FAILED");
  }

  // Persist total regularly
  static uint32_t lastPersistMs = 0;
  if ((uint32_t)(now - lastPersistMs) >= PERSIST_INTERVAL_MS) {
    lastPersistMs = now;
    prefs.begin(NVS_NAMESPACE, false);
    prefs.putUInt(NVS_KEY_TOTAL_MWH, kWhToMWh(total_kwh));
    prefs.end();
  }
}

//
// ============================================================
// ===================== GPIO2 State ==========================
// ============================================================
//

void handleGpio2Change() {
  int s = digitalRead(PIN_SENSOR);
  if (s != last_gpio_state) {
    last_gpio_state = s;

    Serial.printf("GPIO2 state=%s\r\n", (s == LOW) ? "LOW" : "HIGH");
    
    if (mqtt.connected()) {
      bool ok = mqtt.publish(MQTT_TOPIC_GPIO2, (s == LOW) ? "LOW" : "HIGH", true);
    
      Serial.printf("MQTT publish -> %s\r\n", MQTT_TOPIC_GPIO2);
      Serial.printf("Payload: %s\r\n", (s == LOW) ? "LOW" : "HIGH");
      Serial.printf("Result: %s\r\n\r\n", ok ? "OK" : "FAILED");
    }
  }
}

//
// ============================================================
// ===================== LED ==================================
// ============================================================
//

void updateStatusLed() {
  // start red pulse if valid ISR pulse happened
  if (red_pulse_pending) {
    noInterrupts();
    red_pulse_pending = false;
    interrupts();

    redActive = true;
    redStartMs = millis();
  }

  // red has priority
  if (redActive) {
    if ((uint32_t)(millis() - redStartMs) < RED_PULSE_DURATION_MS) {
      setLed(LED_RED_R, LED_RED_G, LED_RED_B);
      return;
    } else {
      redActive = false;
    }
  }

  // online = green
  if (mqttOnline) {
    setLed(LED_GREEN_R, LED_GREEN_G, LED_GREEN_B);
    return;
  }

  // offline = off
  setLed(LED_OFF_R, LED_OFF_G, LED_OFF_B);
}

//
// ============================================================
// ===================== BOOT Button ==========================
// ============================================================
//

void handleBootButton() {
  static bool lastBtn = true;
  bool btn = digitalRead(PIN_BUTTON); // HIGH=released, LOW=pressed

  if (lastBtn && !btn) {
    // reset totals + hour
    noInterrupts();
    pulses_total = 0;
    pulses_hour = 0;
    pending_pulses = 0;
    red_pulse_pending = false;
    interrupts();

    total_kwh_offset = 0.0f;
    hourStartMs = millis();
    
    prefs.begin(NVS_NAMESPACE, false);
    prefs.putUInt(NVS_KEY_TOTAL_MWH, 0);
    prefs.end();
    
    Serial.print("BOOT: reset total+hour\r\n");
    publishState(true);
  }

  lastBtn = btn;
}

//
// ============================================================
// ===================== SETUP ================================
// ============================================================
//

void setup() {
  Serial.begin(115200);
  delay(300);

  Serial.print("\r\n=== GasCounter WiFi MQTT ===\r\n");

  pinMode(PIN_SENSOR, INPUT_PULLUP);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR), isrPulse, FALLING);

  strip.begin();
  strip.setBrightness(LED_BRIGHTNESS);
  setLed(LED_OFF_R, LED_OFF_G, LED_OFF_B);

  // Load persisted total offset (kWh)
  prefs.begin(NVS_NAMESPACE, false);
  total_kwh_offset = mWhToKWh(prefs.getUInt(NVS_KEY_TOTAL_MWH, 0));
  prefs.end();

  hourStartMs = millis();

  wifiConnect();
  mqttConnect();

  last_gpio_state = digitalRead(PIN_SENSOR);
  Serial.printf("GPIO2 state=%s\r\n", (last_gpio_state == LOW) ? "LOW" : "HIGH");

  publishState(true);
}

//
// ============================================================
// ===================== LOOP =================================
// ============================================================
//

void loop() {
  updateStatusLed();
  handleGpio2Change();
  handleBootButton();

  // keep WiFi/MQTT alive
  if (WiFi.status() != WL_CONNECTED) {
    mqttOnline = false;
    wifiConnect();
  }

  if (!mqtt.connected()) {
    mqttOnline = false;
    mqttConnect();
  }
  mqtt.loop();

  // publish when pulses arrived (force), but rate-limited in publishState()
  if (pending_pulses) {
    noInterrupts();
    pending_pulses = 0;
    interrupts();
    publishState(true);
  }

  // periodic publish
  publishState(false);

  // hour rollover (A: hour since boot)
  uint32_t now = millis();
  if ((uint32_t)(now - hourStartMs) >= HOUR_INTERVAL_MS) {
    hourStartMs += HOUR_INTERVAL_MS;
    noInterrupts();
    pulses_hour = 0;
    interrupts();

    Serial.print("Hour rollover -> reset hour pulses\r\n");
    publishState(true);
  }

  delay(MAIN_LOOP_DELAY_MS);
}
