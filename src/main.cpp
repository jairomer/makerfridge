#include <Arduino.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
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
machine_t *machine_state; 
int selected_product;

#define MACHINE_STATS_LEN 256
char machine_stats_buffer[MACHINE_STATS_LEN];

// Callback function header
void callback(char* topic, byte* payload, unsigned int length) {
    board->log("MQTT Callback Called\n");
}

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
    // Loop until we are connected
    while (!client.connected()) {
        board->log("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("ESP8266Client")) {
            board->log("connected\n");
            // Subscribe
            client.subscribe("esp32/output");
        } else {
            board->log("failed, rc=");
            board->log(client.state());
            board->log(" try again in 5 seconds\n");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup() {
    board = new BoardFrameworkArduino();
    machine_state = new Machine(board);

    Serial.begin(115200);

    // Connect ESP32 to the defined access point
    WiFi.mode(WIFI_STA); 
    WiFi.begin(wifi_ssid, wifi_pass);

    board->log("\nConnecting...");
    while(WiFi.status() != WL_CONNECTED) {
        board->log(".");
        delay(100);
    }
    board->log("\nConnected to the WiFi network");
    board->log("Local ESP32 IP: ");
    board->log(WiFi.localIP().toString().c_str());

    // Setup MDNS
    if (!MDNS.begin(mdns_addr)) {
        board->log("Error starting mDNS");
        return;
    }
    board->log("mDNS responder started");

    // Setup connection to MQTT broker.
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    board->log("Reading buttons...\n");
    
    machine_state->read_buttons();
    
    selected_product = machine_state->deliver_product();
    if (selected_product != -1) {
        bool error = machine_state->to_json(machine_stats_buffer, MACHINE_STATS_LEN);
        if (not error) {
            board->log("Publishing stock statistics to 'smartfridge-stock'\n");
            client.publish("smartfridge-stock", machine_stats_buffer);
        } else {
            board->log("[ERROR] Bufferflow detected while serializing machine stock statistics.\n");
        }
    }
}
