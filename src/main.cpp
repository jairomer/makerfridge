#include "Arduino.h"
#include <WiFi.h>

const int TOTAL_PRODUCTS = 5;
const int DIG_INPUT_RANGE = 1;
const int DIG_OUTPUT_RANGE = DIG_INPUT_RANGE+DIG_INPUT_RANGE;

typedef struct ProductPins
{
    int button;
    int actuator;
} product_pins_t;

typedef struct ProductStats
{
    int current_stock;
} product_stats_t;

typedef struct Product
{
    int previous_button_state;
    product_pins_t pins;
    product_stats_t stats;
} product_t;

typedef struct Machine
{
    Machine() {
        // Initialize Machine State
        for (int i=0; i<TOTAL_PRODUCTS; i++) {
            MachineProducts[i].pins.button = DIG_INPUT_RANGE + i;
            MachineProducts[i].previous_button_state = LOW;
            MachineProducts[i].pins.actuator = DIG_OUTPUT_RANGE + i;
            MachineProducts[i].stats.current_stock = 0;
        }
    }

    void read_buttons() {
        for (int i=0; i<TOTAL_PRODUCTS; i++) {
            int previous_button_state = MachineProducts[i].previous_button_state;
            int current_button_state = digitalRead(MachineProducts[i].pins.button);
            if (previous_button_state == LOW && current_button_state == HIGH) {
                Serial.print("Button pressed for product ");
                Serial.println(i);
                MachineProducts[i].previous_button_state = current_button_state;
                // TODO: Actuate product delivery system.
                // TODO: Decrease the stock.
            }
        }
    }

    private: product_t MachineProducts[TOTAL_PRODUCTS];
} machine_t;


machine_t *machineState = new Machine();

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
    machineState->read_buttons();
}
