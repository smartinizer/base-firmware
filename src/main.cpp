#include <Arduino.h>
#include "wifi.hh"

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println();
    Serial.println();

    setupWifi();
}

void loop() {
    // This call takes care of it all
    loopWifi();

    // Small delay to give some breath
    delay(10);
}

