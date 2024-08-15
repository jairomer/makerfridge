#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <PubSubClient.h>

#include "board_framework.hpp"
#include "board_framework_arduino.hpp"
#include "machine.hpp"

#define STR(x) #x
#define XSTR(x) STR(x)

const char* wifi_ssid = XSTR(WIFI_SSID);
const char* wifi_pass = XSTR(WIFI_PASS);
const char* mdns_addr = XSTR(MDNS_ADDR);
const char* ota_pass = XSTR(OTA_PASS);
const char* mqtt_broker = XSTR(MQTT_BROKER);
const uint16_t mqtt_port = MQTT_PORT;

BoardFramework* board;
machine_t *machineState; 

// Callback function header
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.println("MQTT Callback Called");
}

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
    // Loop until we are connected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("ESP8266Client")) {
            Serial.println("connected");
            // Subscribe
            client.subscribe("esp32/output");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

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

    // Setup connection to MQTT broker.
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    Serial.println("Reading buttons...");
    machineState->read_buttons();
    machineState->deliver_product();
    client.publish("test_topic", "hello world");
    delay(100);
}
