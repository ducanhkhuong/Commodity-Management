#ifndef SIMPLE_STEPPER_H
#define SIMPLE_STEPPER_H

#include <Arduino.h>
#include "Timer/TimerMicros.hpp"

class SimpleStepper {
private:
    uint8_t _stepPin, _dirPin;
    long _currentStep = 0;
    long _targetStep = 0;
    bool _dir = true;
    float _speed_mm_s = 1.0;
    unsigned long _interval_us = 10000;

    TimerMicros _timer;
    bool _isMoving = false;

    const float STEPS_PER_MM = 100.0f; // 1mm = 100 bước (bước ren 2mm / 200 bước)

public:
    SimpleStepper(uint8_t stepPin, uint8_t dirPin);
    
    void setSpeed(float mm_s);          // tốc độ mm/s
    void moveTo(float mm);              // vị trí tuyệt đối
    void moveBy(float mm);              // vị trí tương đối
    void stop();
    void update();                      // chạy trong loop()
    bool isRunning();
    float getCurrentPositionMM();
};

#endif
