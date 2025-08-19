#include "Button.hpp"

Button::Button(volatile uint8_t& ddr, volatile uint8_t& port, volatile uint8_t& pin, uint8_t bit)
    : _ddr(ddr), _port(port), _pin(pin), _bit(bit), _lastState(false), _pressCount(0)
{}

void Button::begin() {
    _ddr &= ~(1 << _bit);
    _port |= (1 << _bit);
}

bool Button::isPressed() {
    return !(_pin & (1 << _bit));
}


void Button::updatePressState() {
    bool currentPressed = !(_pin & (1 << _bit));
    if (_lastState && !currentPressed) {
        _pressCount++;
        if (_pressCount > 30) _pressCount = 0;
    }
    _lastState = currentPressed;
}

int Button::getPressCount() {
    return _pressCount;
}