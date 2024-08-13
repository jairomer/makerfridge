#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "board_framework.hpp"
#include <vector>

const int TOTAL_PRODUCTS = 5;

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
    Machine(const BoardFramework* boardfw);
    void read_buttons();
    void set_product_stats(const std::vector<product_stats_t>& newStats);

    product_t machine_products[TOTAL_PRODUCTS];
    const BoardFramework *board;

} machine_t;

#endif // MACHINE_HPP
