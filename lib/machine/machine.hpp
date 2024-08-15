#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "board_framework.hpp"
#include <vector>

const int TOTAL_PRODUCTS = 5;

typedef struct ProductPins
{
    int button = 0;
    int actuator = 0;
} product_pins_t;

typedef struct ProductStats
{
    int current_stock = 0;

} product_stats_t;

typedef struct Product
{
    int previous_button_state = HIGH;
    product_pins_t pins;
    product_stats_t stats;
    bool is_set_for_delivery = false;
} product_t;


typedef struct Machine
{
    Machine(const BoardFramework* boardfw);
    void read_buttons();
    void set_product_stats(const std::vector<product_stats_t>& newStats);
    int deliver_product();
    bool has_products_to_deliver() const;
    bool to_json(char* json_buffer, std::size_t buflen) const;

    product_t machine_products[TOTAL_PRODUCTS];

    const BoardFramework *board;

} machine_t;

#endif // MACHINE_HPP
