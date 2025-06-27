#include "CtrManager.hpp"

CtrManager::CtrManager(uint32_t baurate) :
    ButtonAxisX(DDRC,PORTC,PINC,BUTTON_AXIS_X),
    ButtonAxisY(DDRC,PORTC,PINC,BUTTON_AXIS_Y),
    ButtonAxisZ(DDRC,PORTC,PINC,BUTTON_AXIS_Z),
    StepperX(MOTOR_STEP_X,MOTOR_DIR_X),
    StepperY(MOTOR_STEP_Y,MOTOR_DIR_Y),
    StepperZ(MOTOR_STEP_Z,MOTOR_DIR_Z),
    LedNotify(&PORTB,LED_TEST)
{
    init();//not delete

    //Streaming begin
    stream.begin(baurate);

    //Hardware begin
    ButtonAxisX.begin();
    ButtonAxisY.begin();
    ButtonAxisZ.begin();
    LedNotify.begin();

    //Thread begin
    HandlerUartTransmitTimer.start();
    HandlerUartRecieveTimer.start();
    HandlerNotifyTimer.start();
    HandlerHardWareTimer.start();
    ProcessFoward.start(); 
}

void CtrManager::HandllerPull() {
    if(HandlerUartRecieveTimer.isRunning()){
        if (HandlerUartRecieveTimer.isCheckTime(TIMER_TICK_HANDLER)){
            stream.read();
            dataQueue.bufferPull = stream.getData();
            dataQueue.length = stream.length();
            if (dataQueue.length <= BUFFER_SIZE) {
                //config
                if(dataQueue.bufferPull[0] == 0x1F)
                {
                    if(dataQueue.bufferPull[1]==0xFF){
                        syncMachine.QueueByteConfirm[0] = dataQueue.bufferPull[1];
                        syncMachine.QueueByteConfirm[1] = 0x00; 
                    }else if(dataQueue.bufferPull[1]==0xAA){
                        syncMachine.QueueByteConfirm[0] = dataQueue.bufferPull[1];
                        syncMachine.QueueByteConfirm[1] = 0x00;
                    }else if(dataQueue.bufferPull[1]==0xBB){
                        syncMachine.QueueByteConfirm[0] = dataQueue.bufferPull[1];
                        syncMachine.QueueByteConfirm[1] = 0x00;
                    }else if(dataQueue.bufferPull[1]==0xCC){
                        syncMachine.QueueByteConfirm[0] = dataQueue.bufferPull[1];
                        syncMachine.QueueByteConfirm[1] = 0x00;
                    }
                }
                //controll Send
                if(dataQueue.bufferPull[0]==0x2F && dataQueue.bufferPull[2]==0x1F && dataQueue.bufferPull[3]==0xAA)
                {
                    if(dataQueue.bufferPull[1]==0x01){
                        syncMachine.QueueByteConfirm[1] = dataQueue.bufferPull[1];
                    }else if(dataQueue.bufferPull[1]==0x02){
                        syncMachine.QueueByteConfirm[1] = dataQueue.bufferPull[1];
                    }else if(dataQueue.bufferPull[1]==0x03){
                        syncMachine.QueueByteConfirm[1] = dataQueue.bufferPull[1];
                    }else if(dataQueue.bufferPull[1]==0x04){
                        syncMachine.QueueByteConfirm[1] = dataQueue.bufferPull[1];
                    }
                }
                //controll Recv
                if(dataQueue.bufferPull[0]==0x2F && dataQueue.bufferPull[2]==0x1F && dataQueue.bufferPull[3]==0xBB)
                {
                    if(dataQueue.bufferPull[1]==0x01){
                        syncMachine.QueueByteConfirm[1] = dataQueue.bufferPull[1];
                    }else if(dataQueue.bufferPull[1]==0x02){
                        syncMachine.QueueByteConfirm[1] = dataQueue.bufferPull[1];
                    }else if(dataQueue.bufferPull[1]==0x03){
                        syncMachine.QueueByteConfirm[1] = dataQueue.bufferPull[1];
                    }else if(dataQueue.bufferPull[1]==0x04){
                        syncMachine.QueueByteConfirm[1] = dataQueue.bufferPull[1];
                    }
                }
            }else{
                syncMachine.QueueByteConfirm[0] = 0x00;
                syncMachine.QueueByteConfirm[1] = 0x00;  
            }
        }
    }
}

void CtrManager::HandllerPush() {
    if (HandlerUartTransmitTimer.isRunning()) {
        if (HandlerUartTransmitTimer.isCheckTime(TIMER_TICK_CALLBACK_RESPONSE)) {
            stream.write(hwStatus.bufferPush, sizeof(hwStatus.bufferPush));
        }
    }
}



void CtrManager::HandllerNotify() {
    if(HandlerNotifyTimer.isRunning()){
        if(HandlerNotifyTimer.isCheckTime(TIMER_TICK_HANDLER)){
            ;
        }
    }
}

void CtrManager::HandllerHardware(){
    if(HandlerHardWareTimer.isRunning()){
        if(HandlerHardWareTimer.isCheckTime(TIMER_TICK_CALLBACK_CHECK_HW)){
            //check hardware 
            ButtonAxisX.updatePressState();
            ButtonAxisY.updatePressState();
            ButtonAxisZ.updatePressState();

            hwStatus.buttonX = ButtonAxisX.isPressed();
            hwStatus.buttonY = ButtonAxisY.isPressed();
            hwStatus.buttonZ = ButtonAxisZ.isPressed();
            hwStatus.motorX = StepperX.isRunning();
            hwStatus.motorY = StepperY.isRunning();
            hwStatus.motorZ = StepperZ.isRunning();

            //impl push status hardware byte 
            hwStatus.bufferPush[0] = 0x3F;
            hwStatus.bufferPush[1] = hwStatus.buttonX ? 0x01 : 0x00;
            hwStatus.bufferPush[2] = hwStatus.buttonY ? 0x01 : 0x00;
            hwStatus.bufferPush[3] = hwStatus.buttonZ ? 0x01 : 0x00;
            hwStatus.bufferPush[4] = hwStatus.motorX  ? 0x01 : 0x00;
            hwStatus.bufferPush[5] = hwStatus.motorY  ? 0x01 : 0x00;
            hwStatus.bufferPush[6] = hwStatus.motorZ  ? 0x01 : 0x00;
        }
    }
}

void CtrManager::_ProcessFunc() {
    if(ProcessFoward.isRunning()){
        if(ProcessFoward.isCheckTime(TIMER_TICK_HANDLER)){
            //home
            if(syncMachine.QueueByteConfirm[0]==0xFF && syncMachine.QueueByteConfirm[1]==0x00){
                //start timer home 
            }
            //send
            if(syncMachine.QueueByteConfirm[0]==0xAA && syncMachine.QueueByteConfirm[1]==0x00){
                if(syncMachine.QueueByteConfirm[1]==0x01){

                }else if(syncMachine.QueueByteConfirm[1]==0x02){

                }else if(syncMachine.QueueByteConfirm[1]==0x03){

                }else if(syncMachine.QueueByteConfirm[1]==0x04){

                }
            }
            //recv
            if(syncMachine.QueueByteConfirm[0]==0xBB && syncMachine.QueueByteConfirm[1]==0x00){
                if(syncMachine.QueueByteConfirm[1]==0x01){

                }else if(syncMachine.QueueByteConfirm[1]==0x02){

                }else if(syncMachine.QueueByteConfirm[1]==0x03){

                }else if(syncMachine.QueueByteConfirm[1]==0x04){
                    
                }
            }
            //test
            if(syncMachine.QueueByteConfirm[0]==0xCC && syncMachine.QueueByteConfirm[1]==0x00){
                ;
            }
            //null (lock motor)
            if(syncMachine.QueueByteConfirm[0]==0x00 && syncMachine.QueueByteConfirm[1]==0x00){
                ;
            }
        }
    }
}

void CtrManager::HandllerManager() {
    LedNotify.update();
    HandllerPull();
    HandllerPush();
    HandllerNotify();
    HandllerHardware();
    _ProcessFunc();
}

