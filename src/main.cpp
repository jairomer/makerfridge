#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <ArduinoOTA.h>

#include "board_framework.hpp"
#include "board_framework_arduino.hpp"
#include "machine.hpp"

#define STR(x) #x
#define XSTR(x) STR(x)

const char* wifi_ssid = XSTR(WIFI_SSID);
const char* wifi_pass = XSTR(WIFI_PASS);
const char* mdns_addr = XSTR(MDNS_ADDR);
const char* ota_pass = XSTR(OTA_PASS);

BoardFramework* board;
machine_t *machineState; 

void setup() {
    board = new BoardFrameworkArduino();
    machineState = new Machine(board);

    Serial.begin(115200);

    // Connect ESP32 to the defined access point
    WiFi.mode(WIFI_STA); 
    WiFi.begin(wifi_ssid, wifi_pass);

    Serial.println("\nConnecting...");
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    // Setup MDNS
    if (!MDNS.begin(mdns_addr)) {
        Serial.println("Error starting mDNS");
        return;
    }
    Serial.println("mDNS responder started");

    // Setup OTA
    ArduinoOTA.begin(WiFi.localIP(), mdns_addr, ota_pass, InternalStorage);
}

void loop() {
    Serial.println("Reading buttons...");
    machineState->read_buttons();
    ArduinoOTA.poll();
    delay(100);
}
