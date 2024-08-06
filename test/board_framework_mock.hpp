#ifndef BOARDFRAMEWORKMOCK_HPP
#define BOARDFRAMEWORKMOCK_HPP

#include <stdio.h>

#include "board_framework.hpp"

class BoardFrameworkMock : public BoardFramework {
    public:
        virtual ~BoardFrameworkMock() = default;
        inline void write(int pin, int state) const {
            printf("Writting to pin %d the state %d\n", pin, state);
        }
        inline int read(int pin) const {
            printf("Reading from pin %d the state %d\n", pin, HIGH);
            return HIGH;
        }
        inline void log(const char* message) const { printf("%s", message); }
        inline void fdelay(int ms) const {
            printf("Sleep for %d (not really)\n", ms); 
        }
};

#endif // BOARDFRAMEWORKMOCK_HPP
