#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <avr/io.h>

class Button {
public:
    Button(volatile uint8_t& ddr, volatile uint8_t& port, volatile uint8_t& pin, uint8_t bit);

    void begin();
    bool isPressed();      
    void updatePressState();
    int  getPressCount();
private:
    volatile uint8_t& _ddr;
    volatile uint8_t& _port;
    volatile uint8_t& _pin;
    uint8_t _bit;

    bool _lastState;
    int _pressCount = 0;
};

#endif
