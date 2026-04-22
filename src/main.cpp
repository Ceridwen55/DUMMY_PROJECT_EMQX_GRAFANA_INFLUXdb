#include "MQTT.h"
#include "sensor.h"
#include "wifi_manager.h"
#include <Arduino.h>
#include <WiFi.h>
#include <string>

#define AP_NAME "MyDevice"
#define MQTT_BROKER "ip-vps-lu" // CHANGE WITH VPS IP
#define MQTT_PORT 1883
#define MQTT_USER "user_emqx" // MAKE SURE MATCH EMQX CODE
#define MQTT_PASS "pass_emqx"
#define READ_INTERVAL_MS 5000

static char mqttClientId[32];

static void buildMqttClientId() {
  String mac = WiFi.macAddress();
  mac.replace(":", "");
  snprintf(mqttClientId, sizeof(mqttClientId), "xiao%s", mac.c_str());
}

void setup() {

  Serial.begin(115200); // Baud rate
  delay(1000);

  initWiFi(AP_NAME); // Initiate Wifi

  buildMqttClientId();
  initMQTT(MQTT_BROKER, MQTT_PORT, mqttClientId, MQTT_USER,
           MQTT_PASS); // Initiate MQTT

  initSensor(); // Initiate Sensor
}

void loop() {

  if (!isWiFiConnected()) {
    reconnectWiFi();
    delay(5000);
    return;
  }

  mqttLoop();

  float temperature, humidity; // declare data from sensor
  if (readSensor(temperature, humidity)) {

    publishData(temperature, humidity);
  } else {

    Serial.println("Sensor FAIL.");
  }

  delay(READ_INTERVAL_MS);
}
