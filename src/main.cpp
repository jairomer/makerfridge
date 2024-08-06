#include <Arduino.h>
#include <WiFi.h>

#include "board_framework.hpp"
#include "board_framework_arduino.hpp"
#include "machine.hpp"

BoardFramework* board;
machine_t *machineState; 

void setup() {
    board = new BoardFrameworkArduino();
    machineState = new Machine(board);

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
    machineState->read_buttons();
    delay(50);
}
