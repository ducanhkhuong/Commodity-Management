#include "SimpleStepper.hpp"

SimpleStepper::SimpleStepper(uint8_t stepPin, uint8_t dirPin)
    : _stepPin(stepPin), _dirPin(dirPin) {
    pinMode(_stepPin, OUTPUT);
    pinMode(_dirPin, OUTPUT);
}

void SimpleStepper::setSpeed(float mm_s) {
    if (mm_s <= 0.0f) mm_s = 1.0f;
    _speed_mm_s = mm_s;
    float steps_per_s = mm_s * STEPS_PER_MM;
    _interval_us = (unsigned long)(1000000.0f / steps_per_s);
}

void SimpleStepper::moveTo(float mm) {
    long target = (long)(mm * STEPS_PER_MM);
    long delta = target - _currentStep;

    _dir = (delta >= 0);
    digitalWrite(_dirPin, _dir);
    _targetStep = target;
    _isMoving = true;
    _timer.start();
}

void SimpleStepper::moveBy(float mm) {
    moveTo((_currentStep / STEPS_PER_MM) + mm);
}

void SimpleStepper::stop() {
    _isMoving = false;
    _timer.stop();
}

bool SimpleStepper::isRunning() {
    return _isMoving;
}

void SimpleStepper::update() {
    if (!_isMoving || !_timer.isRunning()) return;

    if (_timer.isCheckTime(_interval_us)) {
        if (_currentStep == _targetStep) {
            _isMoving = false;
            _timer.stop();
            return;
        }

        digitalWrite(_stepPin, HIGH);
        delayMicroseconds(2);
        digitalWrite(_stepPin, LOW);

        _currentStep += (_dir ? 1 : -1);
    }
}

float SimpleStepper::getCurrentPositionMM() {
    return _currentStep / STEPS_PER_MM;
}



// void CtrManager::_ProcessFunc() {
//     static bool isSequential = true; // hoặc lấy trạng thái từ lệnh UART
//     static bool started = false;

//     if(ProcessFoward.isRunning()){
//         if(ProcessFoward.isCheckTime(TIMER_TICK_HANDLER)){
//             if(isSequential) {
//                 // Chạy tuần tự
//                 if(!started) {
//                     StepperX.moveTo(40);
//                     started = true;
//                 }
//                 if(StepperX.isRunning()) StepperX.update();
//                 else if(StepperY.getCurrentPositionMM() != 20) {
//                     StepperY.moveTo(20);
//                 } else if(StepperY.isRunning()) StepperY.update();
//                 else if(StepperZ.getCurrentPositionMM() != 10) {
//                     StepperZ.moveTo(10);
//                 } else if(StepperZ.isRunning()) StepperZ.update();
//                 else started = false; // Kết thúc chu trình
//             } else {
//                 // Chạy song song
//                 if(!started) {
//                     StepperX.moveTo(0);
//                     StepperY.moveTo(0);
//                     StepperZ.moveTo(0);
//                     started = true;
//                 }
//                 if(StepperX.isRunning() || StepperY.isRunning() || StepperZ.isRunning()) {
//                     StepperX.update();
//                     StepperY.update();
//                     StepperZ.update();
//                 } else {
//                     started = false; // Kết thúc chu trình
//                 }
//             }
//         }
//     }
// }