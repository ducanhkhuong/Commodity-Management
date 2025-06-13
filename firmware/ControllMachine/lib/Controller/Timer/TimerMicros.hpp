#pragma once
#include <Arduino.h>

class TimerMicros {
  public:
    TimerMicros();
    void start();
    void stop();
    bool isRunning();
    bool isCheckTime(unsigned long interval);
  private:
    unsigned long _last;
    bool          _running;
};
