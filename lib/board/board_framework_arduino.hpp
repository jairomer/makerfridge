
#ifndef BOARD_FRAMEWORK_ARDUINO_HPP
#define BOARD_FRAMEWORK_ARDUINO_HPP

#include "board_framework.hpp"
#include <Arduino.h>

typedef struct BoardFrameworkArduino : public BoardFramework
{
    public:
        void write(int pin, int state) const { digitalWrite(pin, state); }
        int read(int pin) const { return digitalRead(pin); }
        void log(const char* message) const { Serial.print(message); }
        void fdelay(int ms) const { return delay(ms); }
} board_framework_t;

#endif
