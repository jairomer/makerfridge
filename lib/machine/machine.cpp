#include "machine.hpp"
#include "board_framework.hpp"
#include <stdio.h>
#include <strings.h>

Machine::Machine(const BoardFramework* boardfw) : board(boardfw) {
    // Initialize Machine State
    //
    for (int i=0; i<TOTAL_PRODUCTS; i++) {
        machine_products[i].previous_button_state = HIGH;
        machine_products[i].stats.current_stock = 0;
        machine_products[i].is_set_for_delivery = false;
    }

    // We want to assure consecutive cabling for buttons and actuators.
    machine_products[0].pins.button = 27;
    board->pinmode(machine_products[0].pins.button, PIN_MODE::IN_PULLUP);
    machine_products[0].pins.actuator = 11;
    
    machine_products[1].pins.button = 32;
    board->pinmode(machine_products[1].pins.button, PIN_MODE::IN_PULLUP);
    machine_products[1].pins.actuator = 12;
    
    machine_products[2].pins.button = 33;
    board->pinmode(machine_products[2].pins.button, PIN_MODE::IN_PULLUP);
    machine_products[2].pins.actuator = 14;

    machine_products[3].pins.button = 25;
    board->pinmode(machine_products[3].pins.button, PIN_MODE::IN_PULLUP);
    machine_products[3].pins.actuator = 16;

    machine_products[4].pins.button = 26;
    board->pinmode(machine_products[4].pins.button, PIN_MODE::IN_PULLUP);
    machine_products[4].pins.actuator = 11;
}

/**
 * The purpose of this method is to read the state of the buttons.
 *
 * After a button is activated, the product will be set for delivery.
 *
 * If the product is set for delivery, then the button state will not be processed.
 *
 * At each point we can only select one product for delivery, so if we have products to deliver,
 * no more readings will occur.
 *
 * */
void Machine::read_buttons()
{
    char message[100];
    bzero(&message, 100);

    // Precondition: Only read buttons if there are products to deliver.
    if (this->has_products_to_deliver()) {
        sprintf(message, "Machine has products to deliver, skipping button reading.\n");
        board->log(message);
        return;
    }
    
    for (int i=0; i<TOTAL_PRODUCTS; i++) {
        // Buttons in this machine have a resting state of HIGH and a pressed state of LOW.
        int current_button_state = board->read(machine_products[i].pins.button);

        if (current_button_state == LOW) {
            sprintf(message, "Status for product %d: %d\n", i, current_button_state);
            board->log(message);
            // set product for deliver
            if (machine_products[i].stats.current_stock > 0) {
                sprintf(message, "Setting product %d for delivery.\n", i);
                board->log(message);
                machine_products[i].is_set_for_delivery = true;
                return;
            } else {
                sprintf(message, "Product %d is out of stock.\n", i);
                board->log(message);
                return;
            }
        } else {
            // Store current button state for next iteration.
            machine_products[i].previous_button_state = current_button_state;
        }
    }
}

void Machine::set_product_stats(const std::vector<product_stats_t>& newStats)
{
    char message[100];
    bzero(&message, 100);

    if (newStats.size() != TOTAL_PRODUCTS) {
        // Verify preconditions
        sprintf(
                message,
                "Received incorrect number of stats, expecting %d but got %d.\n",
                TOTAL_PRODUCTS, (int)newStats.size());
        board->log(message);
        return;
    }
    for (int i=0; i<TOTAL_PRODUCTS; i++) { 
        machine_products[i].stats.current_stock = newStats[i].current_stock;
    }
    board->log("Statistics for machine products got updated.\n");
}


/**
 * Will do the necessary operations for a product marked for delivery.
 *
 * Return -1 if no product has been delivered. 
 * */
int Machine::deliver_product() {
    for (int i=0; i<TOTAL_PRODUCTS; i++) {
        if (machine_products[i].is_set_for_delivery) {

            // Actions uppon delivery.
            //
            // 1. Reset marked for delivery flag
            machine_products[i].is_set_for_delivery = false;
            // 2. Decrement the stock.
            if (machine_products[i].stats.current_stock > 0) {
                machine_products[i].stats.current_stock -= 1;
            }

            return i;
        }
    }
    return -1;
}

bool Machine::has_products_to_deliver() const {
    for (int i=0; i<TOTAL_PRODUCTS; i++) {
        if (machine_products[i].is_set_for_delivery) {
            return true;
        }
    }
    return false;
}
