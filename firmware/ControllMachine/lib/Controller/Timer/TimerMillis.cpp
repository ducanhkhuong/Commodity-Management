#include "TimerMillis.hpp"

TimerMillis::TimerMillis() : _last(0), _running(false) {}

void TimerMillis::start()  {
    _last = millis(); 
    _running = true; 
}

void TimerMillis::stop()   {
    _running = false; 
}

bool TimerMillis::isRunning(){ 
    return _running; 
}

bool TimerMillis::isCheckTime(unsigned long interval) {
    if (!_running) return false;               

    unsigned long now = millis();   

    if (now - _last >= interval) {
        _last = now;                           
        return true;                           
    }
    return false;                          
}