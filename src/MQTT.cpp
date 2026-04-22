#include "MQTT.h"
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

// DECLARATION
#define TOPIC_TEMPLATE "devices/%s/telemetry"

static WiFiClient wifiClient;
static PubSubClient mqttClient(wifiClient);

static const char *_broker;   // define MQTT Broker Address
static const char *_clientId; // define Client ID
static const char *_user;     // define username
static const char *_pass;     // define password
static char _topic[64];       // define topic buffer

// RECONNECT TO MQTT BROKER
static bool reconnect_mqtt() { // function to connect to MQTT Broker

  if (mqttClient.connected()) { // check if already connected to broker
    return true;
  }

  Serial.printf("[MQTT] Connecting to %s...\n", _broker); // print broker address

  bool connected = mqttClient.connect(_clientId, _user, _pass); // connect

  if (connected) { // check if connected to broker
    Serial.println("[MQTT] Connected!"); // print connected to broker
  } else {
    Serial.printf("[MQTT] Failed, rc=%d. Try again later.\n",
                  mqttClient.state()); // print connection failed
  }

  return connected; // return connected
}

// INITIATE MQTT
bool initMQTT(
    const char *broker,
    int port, // broker address, port number, client ID, username, password
    const char *clientId, const char *user, const char *pass) {

  _broker = broker;     // set broker address
  _clientId = clientId; // set client ID
  _user = user;         // set username
  _pass = pass;         // set password

  snprintf(_topic, sizeof(_topic), TOPIC_TEMPLATE,
           clientId); // define topic buffer

  mqttClient.setServer(broker, port); // set server address
  mqttClient.setKeepAlive(60);        // set keep alive

  Serial.printf("[MQTT] Topic: %s\n", _topic); // print topic

  return reconnect_mqtt(); // return connected
}

// PUBLISH TO MQTT BROKER

bool publishData(float temperature,
                 float humidity) { // function to publish data to MQTT Broker
  if (!reconnect_mqtt())
    return false; // check if connected to broker

  // Format payload JSON
  char payload[64];
  snprintf(payload, sizeof(payload), "{\"temp\":%.2f,\"hum\":%.2f}",
           temperature, humidity);

  bool ok =
      mqttClient.publish(_topic, payload, true); // publish data to MQTT Broker

  if (ok) {
    Serial.printf("[MQTT] Published: %s\n", payload);
  } else {
    Serial.println("[MQTT] Publish gagal!");
  }

  return ok;
}

void mqttLoop() { // looping for keeping MQTT Alive
  mqttClient.loop();
}
