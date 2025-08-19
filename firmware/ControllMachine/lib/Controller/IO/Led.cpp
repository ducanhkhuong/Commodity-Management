#include "Led.hpp"

Led::Led(volatile uint8_t* port, uint8_t pin)
    : _port(port), _pin(pin) {}

void Led::begin() {
    if (_port == &PORTB) DDRB |= (1 << _pin);
    else if (_port == &PORTC) DDRC |= (1 << _pin);
    else if (_port == &PORTD) DDRD |= (1 << _pin);
}

void Led::on() {
    *_port |= (1 << _pin);
    _state = true;
}

void Led::off() {
    *_port &= ~(1 << _pin);
    _state = false;
}

void Led::toggle() {
    *_port ^= (1 << _pin);
    _state = !_state;
}

void Led::pwmStart(uint8_t duty, uint16_t period_ms) {
    duty = (duty > 100) ? 100 : duty;
    _onTime = (period_ms * duty) / 100;
    _offTime = period_ms - _onTime;
    _isPwm = true;
    on();
    _pwmTimer.start();
}

void Led::pwmStop() {
    _isPwm = false;
    _pwmTimer.stop();
    off();
}

void Led::update() {
    if (!_isPwm || !_pwmTimer.isRunning()) return;
    if (_state && _pwmTimer.isCheckTime(_onTime)) {
        off();
    } else if (!_state && _pwmTimer.isCheckTime(_offTime)) {
        on();
    }
}


// #include "Led.hpp"
// #include "TimerMillis.hpp"

// // Giả sử LED nối với pin D7 → PORTD, pin 7
// Led led(&PORTD, 7);

// void setup() {
//     led.begin();
//     led.pwmStart(50, 1000); // PWM 50% chu kỳ 1000ms (1Hz)
// }

// void loop() {
//     led.update(); // phải gọi liên tục để PWM hoạt động
// }
