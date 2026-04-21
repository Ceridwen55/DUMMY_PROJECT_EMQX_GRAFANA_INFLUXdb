#include "sensor.h"
#include "Arduino.h"
#include "Wire.h"

static Adafruit_SHT31 sht31 = Adafruit_SHT31();

bool initSensor() {
  Wire.begin();
  if (!sht31.begin(0x44)) {
    Serial.println("Error SHT31 NOT FOUND!");
    return false;
  }
  return true;
}

bool readSensor(float &temperature, float &humidity) {
  temperature = sht31.readTemperature();
  humidity = sht31.readHumidity();
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error reading sensor!");
    return false;
  }
  return true;
}