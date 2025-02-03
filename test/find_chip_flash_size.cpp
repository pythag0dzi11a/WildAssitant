#include <Arduino.h>
#include "LittleFS.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.print("Chip ID: ");
  Serial.println(ESP.getFlashChipId());
  Serial.print("Chip Real Size: ");
  Serial.println(ESP.getFlashChipRealSize());
  Serial.print("Chip Size: ");
  Serial.println(ESP.getFlashChipSize());
  Serial.print("Chip Speed: ");
  Serial.println(ESP.getFlashChipSpeed());
  Serial.print("Chip Mode: ");
  Serial.println(ESP.getFlashChipMode());
}

void loop() {
  // Do nothing
}
