#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <Arduino.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <HardwareSerial.h>
#include "Stream.hpp"
#include "Motor/Stepper.hpp"
#include "IO/Button.hpp"
#include "IO/Led.hpp"
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
            uint8_t * bufferPull;
            size_t length;
        };DataQueue dataQueue;

        //Controll Machine
        struct SyncMachine{
            uint8_t QueueByteConfirm[2];
        };SyncMachine syncMachine;

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
            bool motorX;
            bool motorY;
            bool motorZ;
            uint8_t bufferPush[7];
        };HardWare hwStatus;

        //Stepper status
        struct StepperMonitor{
            int stepStage;
            bool moving;
        };StepperMonitor stepperMonitor;

        TimerMillis HandlerUartRecieveTimer;
        TimerMillis HandlerUartTransmitTimer;
        TimerMillis HandlerNotifyTimer;
        TimerMillis HandlerHardWareTimer;
        TimerMillis ProcessFoward;

        Button ButtonAxisX;
        Button ButtonAxisY;
        Button ButtonAxisZ;

        AccelStepper stepperX;
        AccelStepper stepperY;
        AccelStepper stepperZ;

        void HandllerPull();
        void HandllerPush();
        void HandllerNotify();
        void HandllerHardware();
        void _ProcessFunc();
};
