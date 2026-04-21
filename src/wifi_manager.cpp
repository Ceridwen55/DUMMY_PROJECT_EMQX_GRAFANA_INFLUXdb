#include "wifi_manager.h"
#include "WiFiManager.h"
#include <Arduino.h>
#include <WiFi.h>


static WiFiManager wifiManager;

bool initWiFi(const char *apName, int portalTimeoutSec) { // Initialize WiFi and
                                                          // connect to network
  wifiManager.setConfigPortalTimeout(portalTimeoutSec);
  wifiManager.setConnectTimeout(30);

  if (!wifiManager.autoConnect(apName)) { // Auto connect to WiFi network
    Serial.println("[WiFi] Not Connected, restart...");
    delay(1000);
    ESP.restart();
    return false;
  }

  Serial.printf("[WiFi] Connected! IP: %s\n",
                WiFi.localIP().toString().c_str()); // Print IP Address
  return true;
}

bool isWiFiConnected() {
  return WiFi.isConnected();
} // check if wifi is connected

void reconnectWiFi() { // reconnect to wifi if cutoff
  Serial.println("[WiFi] Disconnected, reconnecting...");
  WiFi.reconnect();
}