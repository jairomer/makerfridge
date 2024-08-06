#ifndef BOARD_FRAMEWORK_HPP
#define BOARD_FRAMEWORK_HPP

#define HIGH 0x1
#define LOW 0x0

/***
 * The purpose of this structure is to decouple the controller framework
 * from the machine state in order to make independent tests.
 */
class BoardFramework
{
    public:
        virtual void write(int pin, int state) const;
        virtual int read(int pin) const;
        virtual void log(const char* message) const;
        virtual void fdelay(int ms) const;
};

#endif
