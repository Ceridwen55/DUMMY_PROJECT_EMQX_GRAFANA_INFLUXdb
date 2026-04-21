#pragma once
#include <Adafruit_SHT31.h>

bool initSensor();
bool readSensor(float &temperature, float &humidity);