#include "CtrManager.hpp"

CtrManager::CtrManager(uint32_t baurate) :
    ButtonAxisX(DDRC,PORTC,PINC,BUTTON_AXIS_X),
    ButtonAxisY(DDRC,PORTC,PINC,BUTTON_AXIS_Y),
    ButtonAxisZ(DDRC,PORTC,PINC,BUTTON_AXIS_Z)
{
    init();//not delete
    //Streaming begin
    stream.begin(baurate);
    
    //Hardware begin
    ButtonAxisX.begin();
    ButtonAxisY.begin();
    ButtonAxisZ.begin();

    //Thread begin
    HandlerUartTransmitTimer.start();
    HandlerUartRecieveTimer.start();
    HandlerNotifyTimer.start();
    HandlerHardWareTimer.start();
    ProcessFoward.start(); 
}

void CtrManager::HandllerReceive() {
    if(HandlerUartRecieveTimer.isRunning()){
        if (HandlerUartRecieveTimer.isCheckTime(TIMER_TICK_HANDLER)){
            stream.read();
            dataQueue.dataBuffer = stream.getData();
            dataQueue.length = stream.length();
            //Nhận các byte dữ liệu và parse thành dữ liệu máy học , cập nhật vào kiến trúc đồng bộ (config , controll)
            if (dataQueue.length <= BUFFER_SIZE) {
                uint16_t value = (dataQueue.dataBuffer[0] << 8) | dataQueue.dataBuffer[1];
                (void)value;//kiểu máy học khi parse và ghép byte 
            } else {
                ;
            }  
        }
    }
}

void CtrManager::HandllerTransmit() {
    if(HandlerUartTransmitTimer.isRunning()){
        if(HandlerUartTransmitTimer.isCheckTime(TIMER_TICK_HANDLER)){
            //parse các dữ liệu máy học (int , bool , ...) thành byte và cho vào mảng và gửi đi   
            //ví dụ 
            uint8_t dataBuffer[] = {01, 02, 03, 04};
            stream.write(dataBuffer,sizeof(dataBuffer));
        }
    }
}

void CtrManager::HandllerNotify() {
    if(HandlerNotifyTimer.isRunning()){
        if(HandlerNotifyTimer.isCheckTime(TIMER_TICK_HANDLER)){
            //quét các cấu trúc luồng timer , có phát hiện dữ liệu sai không , gửi cập nhật notify cảnh báo 
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        }
    }
}

void CtrManager::HandllerHardware(){
    if(HandlerHardWareTimer.isRunning()){
        if(HandlerHardWareTimer.isCheckTime(TIMER_TICK_CALLBACK_CHECK_HW)){
            //cập nhật các thông tin , trạng thái phần cứng (motor,button,...)

            //Button check hardware 
            ButtonAxisX.updatePressState();
            ButtonAxisY.updatePressState();
            ButtonAxisZ.updatePressState();

            //Motor check hardware
            //...
            //...
            //...

            //Update hardware button deboucing
            hwStatus.buttonX = ButtonAxisX.isPressed();
            hwStatus.buttonY = ButtonAxisY.isPressed();
            hwStatus.buttonZ = ButtonAxisZ.isPressed();
            hwStatus.counterPress[0] = ButtonAxisX.getPressCount();
            hwStatus.counterPress[1] = ButtonAxisY.getPressCount();
            hwStatus.counterPress[2] = ButtonAxisZ.getPressCount();

            //Update hardware motor deboucing 
            //...
            //...
            //...
        }
    }
}

void CtrManager::_ProcessFunc() {
    if(ProcessFoward.isRunning()){
        if(ProcessFoward.isCheckTime(TIMER_TICK_HANDLER)){
            //excutecommand 
            ;
        }
    }
}

void CtrManager::HandllerManager() {
    HandllerReceive();
    HandllerTransmit();
    HandllerNotify();
    HandllerHardware();
    _ProcessFunc();
}

