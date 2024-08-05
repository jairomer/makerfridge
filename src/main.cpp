#include "Arduino.h"
#include <WiFi.h>

void setup() {
    Serial.begin(115200);

    WiFi.mode(WIFI_STA); // Set ESP32 t station mode
    WiFi.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    Serial.println("Hello World!");
    delay(1000);
}
