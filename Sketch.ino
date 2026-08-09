#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>

// 1. إعدادات شبكة Wi-Fi للمحاكي Wokwi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// 2. إعدادات MQTT Broker
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "home/energy/telemetry";

// 3. إعدادات Telegram Bot
String botToken = "8436121881:AAH2rKbRo-o6Igakp9p-FJOKyYKYk6d_FFA"; 
String chatId = "1442876266";     

// 4. متغيرات وحساسات
const int POT_PIN = 34; 
const float VOLTAGE = 220.0; 
const float MAX_POWER_THRESHOLD = 2000.0; 

WiFiClient espClient;
PubSubClient client(espClient);
bool alertSent = false;

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

void sendTelegramMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    // تجهيز رابط الطلب
    String url = "https://api.telegram.org/bot" + botToken + "/sendMessage?chat_id=" + chatId + "&text=" + message;
    
    // إرسال الطلب عبر HTTPClient
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode > 0) {
      Serial.println(">>> Telegram response code: " + String(httpCode));
    } else {
      Serial.println(">>> Error sending Telegram message: " + http.errorToString(httpCode));
    }
    http.end();
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32EnergyMonitorClient")) {
      Serial.println("MQTT Connected!");
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(POT_PIN, INPUT);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int rawValue = analogRead(POT_PIN);
  float current = (rawValue / 4095.0) * 15.0; 
  float power = VOLTAGE * current; 

  String payload = "{\"voltage\":" + String(VOLTAGE) + 
                   ",\"current\":" + String(current, 2) + 
                   ",\"power\":" + String(power, 2) + "}";

  client.publish(mqtt_topic, payload.c_str());
  Serial.println("Published: " + payload);

  // التحقق من التجاوز
  if (power > MAX_POWER_THRESHOLD && !alertSent) {
    Serial.println(">>> Threshold Exceeded! Sending Telegram Alert...");
    sendTelegramMessage("Alert_Power_Exceeded”);
    alertSent = true;
  } else if (power <= MAX_POWER_THRESHOLD) {
    alertSent = false; 
  }

  delay(3000); 
}
