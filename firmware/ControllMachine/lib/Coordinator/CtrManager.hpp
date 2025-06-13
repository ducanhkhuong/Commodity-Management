#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <Arduino.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <HardwareSerial.h>
#include "Stream.hpp"
#include "Button/Button.hpp"
#include "Motor/Stepper.hpp"
#include "Timer/TimerMillis.hpp"
#include "Timer/TimerMicros.hpp"

class CtrManager{
    public:
        CtrManager(uint32_t baurate);
        void HandllerManager();
    private:
        //Streaming data
        BridgeStream stream;

        //Queue
        struct DataQueue
        {
            uint8_t * dataBuffer;
            size_t length;
        };DataQueue dataQueue;

        //Controll
        struct SyncControl{
            ;
        };

        //Status
        struct SyncStatus{
            ;
        };

        //Config
        struct SyncConfig{
            ;
        };

        //Warning Notify
        struct Notify{
            bool flagWarning;
            bool isActive;
        };

        //Hardware status deboucing
        struct HardWare{
            bool buttonX;
            bool buttonY;
            bool buttonZ;
            int counterPress[3];
        };HardWare hwStatus;

        TimerMillis HandlerUartRecieveTimer;
        TimerMillis HandlerUartTransmitTimer;
        TimerMillis HandlerNotifyTimer;
        TimerMillis HandlerHardWareTimer;
        TimerMillis ProcessFoward;

        Button ButtonAxisX;
        Button ButtonAxisY;
        Button ButtonAxisZ;

        void HandllerReceive();
        void HandllerTransmit();
        void HandllerNotify();
        void HandllerHardware();
        void _ProcessFunc();
};
