#include "TimerMicros.hpp"

TimerMicros::TimerMicros() : _last(0), _running(false) {}

void TimerMicros::start()  {
    _last = micros(); 
    _running = true; 
}

void TimerMicros::stop()   {
    _running = false; 
}

bool TimerMicros::isRunning(){ 
    return _running; 
}

bool TimerMicros::isCheckTime(unsigned long interval) {
    if (!_running) return false;               

    unsigned long now = micros();   

    if (now - _last >= interval) {
        _last = now;                           
        return true;                           
    }
    return false;                          
}