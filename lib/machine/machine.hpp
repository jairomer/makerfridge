#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "board_framework.hpp"
#include <vector>

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
    Machine(const BoardFramework* boardfw) : 
        board(boardfw) {
        // Initialize Machine State
        for (int i=0; i<TOTAL_PRODUCTS; i++) {
            machine_products[i].previous_button_state = LOW;
            machine_products[i].stats.current_stock = 0;
        }

        // TODO: Confirm final pin distribution
        // We want to assure consecutive cabling for buttons and actuators.
        machine_products[0].pins.button = 13;
        machine_products[0].pins.actuator = 15;
        
        machine_products[1].pins.button = 12;
        machine_products[1].pins.actuator = 2;
        
        machine_products[2].pins.button = 14;
        machine_products[2].pins.actuator = 4;

        machine_products[3].pins.button = 27;
        machine_products[3].pins.actuator = 16;

        machine_products[4].pins.button = 26;
        machine_products[4].pins.actuator = 17;
    }

    void read_buttons();
    void set_product_stats(const std::vector<product_stats_t>& newStats);

    private:
        product_t machine_products[TOTAL_PRODUCTS];
        const BoardFramework *board;

} machine_t;

#endif // MACHINE_HPP
