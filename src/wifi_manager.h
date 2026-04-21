#pragma once

bool initWiFi(const char *apName, int portalTimeoutSec = 180);
bool isWiFiConnected();
void reconnectWiFi();