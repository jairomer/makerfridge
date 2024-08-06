#include <unity.h>
#include <vector>

#include "machine.hpp"
#include "board_framework_mock.hpp"

BoardFramework* board;
Machine* machineState;

void setUp(void) {
    board = new BoardFrameworkMock();
    machineState = new Machine(board);
}

void tearDown(void) {
    delete machineState;
}

void test_machine_state_initialization() {
    TEST_ASSERT_NOT_NULL(board);
    TEST_ASSERT_NOT_NULL(machineState);
}

void test_machine_stats_setting() {
    std::vector<product_stats_t> stats;
    for (int i=0; i<TOTAL_PRODUCTS; i++) {
        product_stats_t stat;
        stat.current_stock = 8;
        stats.push_back(stat);
    }
    machineState->set_product_stats(stats);

    for (int i=0; i<TOTAL_PRODUCTS; i++) {
        TEST_ASSERT_EQUAL_INT(8, machineState->machine_products[i].stats.current_stock);
    }
}

void test_machine_state_read() {
   
    // Put 8 items for each product into the machine .
    std::vector<product_stats_t> stats;
    for (int i=0; i<TOTAL_PRODUCTS; i++) {
        product_stats_t stat;
        stat.current_stock = 8;
        stats.push_back(stat);
    }
    machineState->set_product_stats(stats);

    // Mocker should set all buttons to HIGH
    machineState->read_buttons();
    
    for (int i=0; i<TOTAL_PRODUCTS; i++) {
        TEST_ASSERT_EQUAL_INT(HIGH, machineState->machine_products[i].previous_button_state);
        TEST_ASSERT_EQUAL_INT(7, machineState->machine_products[i].stats.current_stock);
    }
   
    machineState->read_buttons();

    // In the next iteration, if buttons are still set to HIGH without passing 
    // through a LOW state, then no product should be delivered.
    for (int i=0; i<TOTAL_PRODUCTS; i++) {
        TEST_ASSERT_EQUAL_INT(HIGH, machineState->machine_products[i].previous_button_state);
        TEST_ASSERT_EQUAL_INT(7, machineState->machine_products[i].stats.current_stock);
    }
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_machine_state_initialization);
    RUN_TEST(test_machine_stats_setting);
    RUN_TEST(test_machine_state_read);
    return UNITY_END();
}


