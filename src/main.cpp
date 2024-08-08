#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>

#include "board_framework.hpp"
#include "board_framework_arduino.hpp"
#include "machine.hpp"

BoardFramework* board;
machine_t *machineState; 

void setup() {
    board = new BoardFrameworkArduino();
    machineState = new Machine(board);

    Serial.begin(115200);
    Serial.println("Waiting 5s...");
    delay(5000);

    // Connect ESP32 to the defined access point
    WiFi.mode(WIFI_STA); 
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    Serial.println("\nConnecting...");
    int i=0;
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        if (i == 100) {
            i = 0;
            Serial.println(WiFi.status());
            Serial.println(WIFI_SSID);
        }
        i++;
        delay(100);
    }
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    if (!MDNS.begin(MDNS_ADDR)) {
        Serial.println("Error starting mDNS");
        return;
    }
    Serial.println("mDNS responder started");

}

void loop() {
    Serial.println("Reading buttons...");
    machineState->read_buttons();
    delay(100);
}
