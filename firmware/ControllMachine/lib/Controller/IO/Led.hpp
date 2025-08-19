#ifndef LED_HPP
#define LED_HPP

#include <avr/io.h>
#include "Timer/TimerMillis.hpp"

class Led {
private:
    volatile uint8_t* _port;
    uint8_t _pin;
    TimerMillis _pwmTimer;
    unsigned long _onTime;
    unsigned long _offTime;
    bool _isPwm = false;
    bool _state = false;

public:
    Led(volatile uint8_t* port, uint8_t pin);
    
    void begin();
    void on();
    void off();
    void toggle();
    void pwmStart(uint8_t duty, uint16_t period_ms);
    void pwmStop();

    void update();
};

#endif
