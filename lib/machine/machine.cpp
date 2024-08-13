#include "machine.hpp"
#include "board_framework.hpp"
#include <stdio.h>

Machine::Machine(const BoardFramework* boardfw) : board(boardfw) {
    // Initialize Machine State
    //
    for (int i=0; i<TOTAL_PRODUCTS; i++) {
        machine_products[i].previous_button_state = HIGH;
        machine_products[i].stats.current_stock = 0;
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

void Machine::read_buttons()
{
    char message[100];
    for (int i=0; i<TOTAL_PRODUCTS; i++) {
        //int previous_button_state = machine_products[i].previous_button_state;
        int current_button_state = board->read(machine_products[i].pins.button);
        sprintf(message, "Status for product %d: %d\n", i, current_button_state);
        board->log(message);

        //if (previous_button_state == LOW && current_button_state == HIGH) {

        //    if (machine_products[i].stats.current_stock > 0) {
        //        machine_products[i].stats.current_stock -= 1;
        //        machine_products[i].previous_button_state = current_button_state;

        //        //board->log("Turning actuator on\n");
        //        //board->write(machine_products[i].pins.actuator, HIGH);
        //        //board->fdelay(500);
        //        //board->write(machine_products[i].pins.actuator, LOW);
        //        //board->log("Turning actuator off\n");

        //    } else {
        //        // TODO: Consider adding a buzzer for feedback.
        //        sprintf(message, "Product %d not available\n", i);
        //        board->log(message);
        //    }
        //} else if (previous_button_state == HIGH && current_button_state == LOW) {
        //    // Turn Actuator off
        //    board->log("Turning actuator off\n");
        //    board->write(machine_products[i].pins.actuator, LOW);
        //    machine_products[i].previous_button_state = LOW;
        //}
    }
}

void Machine::set_product_stats(const std::vector<product_stats_t>& newStats)
{
    if (newStats.size() != TOTAL_PRODUCTS) {
        // Verify preconditions
        char message[100];
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
