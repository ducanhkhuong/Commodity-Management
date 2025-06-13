#pragma once
#include <Arduino.h>

class TimerMillis {
  public:
    TimerMillis();
    void start();
    void stop();
    bool isRunning();
    bool isCheckTime(unsigned long interval);
  private:
    unsigned long _last;
    bool          _running;
};
