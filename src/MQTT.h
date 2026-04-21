#pragma once

bool initMQTT(const char *broker, int port, const char *clientId,
              const char *user, const char *pass);
bool publishData(float temperature, float humidity);
void mqttLoop();