#include "Arduino.h"
#include <WiFi.h>

void setup() {
    Serial.begin(115200);

    // Connect ESP32 to the defined access point
    WiFi.mode(WIFI_STA); 
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    Serial.println("\nConnecting...");
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    Serial.println("Hello World!");
    delay(1000);
}
