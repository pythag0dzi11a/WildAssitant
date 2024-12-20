#include <Arduino.h>

//pin
#define enableSensor 1
#define analogPin 2

void setup() {
    Serial.begin(115200);
    pinMode(enableSensor, OUTPUT);
    pinMode(analogPin, INPUT);
}

void loop() {
    static int soilWet = 0;

    digitalWrite(enableSensor, HIGH);
    soilWet = analogRead(analogPin);
    Serial.println(soilWet);
    digitalWrite(enableSensor, LOW);
    delay(1000);
}
