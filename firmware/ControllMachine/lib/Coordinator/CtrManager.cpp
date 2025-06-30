#include "CtrManager.hpp"

CtrManager::CtrManager(uint32_t baurate) :
    ButtonAxisX(DDRE,PORTE,PINE,BUTTON_AXIS_X),
    ButtonAxisY(DDRJ,PORTJ,PINJ,BUTTON_AXIS_Y),
    ButtonAxisZ(DDRD,PORTD,PIND,BUTTON_AXIS_Z),
    stepperX(AccelStepper::DRIVER, MOTOR_STEP_X, MOTOR_DIR_X),
    stepperY(AccelStepper::DRIVER, MOTOR_STEP_Y, MOTOR_DIR_Y),
    stepperZ(AccelStepper::DRIVER, MOTOR_STEP_Z, MOTOR_DIR_Z)
{
    init();//not delete

    //Streaming begin
    stream.begin(baurate);

    //Hardware begin && enable driver
    ButtonAxisX.begin();
    ButtonAxisY.begin();
    ButtonAxisZ.begin();

    pinMode(MOTOR_ENA_X,OUTPUT);
    pinMode(MOTOR_ENA_Y,OUTPUT);
    pinMode(MOTOR_ENA_Z,OUTPUT);

    digitalWrite(MOTOR_ENA_X,LOW);
    digitalWrite(MOTOR_ENA_Y,LOW);
    digitalWrite(MOTOR_ENA_Z,LOW);

    //Thread begin
    HandlerUartTransmitTimer.start();
    HandlerUartRecieveTimer.start();
    HandlerNotifyTimer.start();
    HandlerHardWareTimer.start();
    ProcessFoward.start(); 

    //config
    stepperX.setMaxSpeed(8000);
    stepperX.setAcceleration(4000);
    stepperX.setCurrentPosition(0);

    stepperY.setMaxSpeed(8000);
    stepperY.setAcceleration(4000);
    stepperY.setCurrentPosition(0);

    stepperZ.setMaxSpeed(8000);
    stepperZ.setAcceleration(4000);
    stepperZ.setCurrentPosition(0);

    stepperX.stop();
    stepperX.stop();
    stepperX.stop();

    stepperMonitor.stepStage = 0;
    stepperMonitor.moving = false;
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
                        syncMachine.QueueByteConfirm[0] = dataQueue.bufferPull[3];
                        syncMachine.QueueByteConfirm[1] = dataQueue.bufferPull[1];
                    }else if(dataQueue.bufferPull[1]==0x02){
                        syncMachine.QueueByteConfirm[0] = dataQueue.bufferPull[3];
                        syncMachine.QueueByteConfirm[1] = dataQueue.bufferPull[1];
                    }else if(dataQueue.bufferPull[1]==0x03){
                        syncMachine.QueueByteConfirm[0] = dataQueue.bufferPull[3];
                        syncMachine.QueueByteConfirm[1] = dataQueue.bufferPull[1];
                    }else if(dataQueue.bufferPull[1]==0x04){
                        syncMachine.QueueByteConfirm[0] = dataQueue.bufferPull[3];
                        syncMachine.QueueByteConfirm[1] = dataQueue.bufferPull[1];
                    }
                }
                //controll Recv
                if(dataQueue.bufferPull[0]==0x2F && dataQueue.bufferPull[2]==0x1F && dataQueue.bufferPull[3]==0xBB)
                {
                    if(dataQueue.bufferPull[1]==0x01){
                        syncMachine.QueueByteConfirm[0] = dataQueue.bufferPull[3];
                        syncMachine.QueueByteConfirm[1] = dataQueue.bufferPull[1];
                    }else if(dataQueue.bufferPull[1]==0x02){
                        syncMachine.QueueByteConfirm[0] = dataQueue.bufferPull[3];
                        syncMachine.QueueByteConfirm[1] = dataQueue.bufferPull[1];
                    }else if(dataQueue.bufferPull[1]==0x03){
                        syncMachine.QueueByteConfirm[0] = dataQueue.bufferPull[3];
                        syncMachine.QueueByteConfirm[1] = dataQueue.bufferPull[1];
                    }else if(dataQueue.bufferPull[1]==0x04){
                        syncMachine.QueueByteConfirm[0] = dataQueue.bufferPull[3];
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
            hwStatus.motorX = stepperX.isRunning();
            hwStatus.motorY = stepperY.isRunning();
            hwStatus.motorZ = stepperZ.isRunning();

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
        if(ProcessFoward.isCheckTime(1)){
            //home
            if(syncMachine.QueueByteConfirm[0]==0xFF && syncMachine.QueueByteConfirm[1]==0x00){
                stepperMonitor.stepStage = 1;
                switch (stepperMonitor.stepStage){
                    case 1:
                        if(!stepperX.isRunning() && !stepperY.isRunning() && !stepperZ.isRunning()){
                            stepperY.moveTo(-10000);
                        }
                        if (stepperY.isRunning() && !ButtonAxisY.isPressed()){
                            stepperY.stop();
                            stepperY.moveTo(5 * STEPS_PER_MM);
                            stepperY.setCurrentPosition(0);
                            stepperMonitor.stepStage = 2;
                        }
                    break;

                    case 2:
                        // go to
                    break;
                }
            }
            //send
            if(syncMachine.QueueByteConfirm[0]==0xAA && syncMachine.QueueByteConfirm[1]==0x01){
                stepperMonitor.stepStage = 1;
                switch (stepperMonitor.stepStage) {
                    case 1:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(70 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 2;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 2:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(70 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 3;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 3:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 4;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 4:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(-260 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 5;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 5:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(70 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 6;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 6:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 7;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 7:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 8;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 8:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 9;
                            stepperMonitor.moving = false;
                        }
                    break;
                    
                    case 9:             
                        stepperMonitor.stepStage = 0;
                        stepperMonitor.moving = false;        
                        stepperX.stop();
                        stepperX.stop();
                        stepperX.stop();
                    break;
                }
            }
            if(syncMachine.QueueByteConfirm[0]==0xAA && syncMachine.QueueByteConfirm[1]==0x02){

            }
            if(syncMachine.QueueByteConfirm[0]==0xAA && syncMachine.QueueByteConfirm[1]==0x03){

            }
            if(syncMachine.QueueByteConfirm[0]==0xAA && syncMachine.QueueByteConfirm[1]==0x04){

            }
            
            //recv
            if(syncMachine.QueueByteConfirm[0]==0xBB && syncMachine.QueueByteConfirm[1]==0x01){

            }
            if(syncMachine.QueueByteConfirm[0]==0xBB && syncMachine.QueueByteConfirm[1]==0x02){

            }
            if(syncMachine.QueueByteConfirm[0]==0xBB && syncMachine.QueueByteConfirm[1]==0x03){

            }
            if(syncMachine.QueueByteConfirm[0]==0xBB && syncMachine.QueueByteConfirm[1]==0x04){

            }

            //test
            if(syncMachine.QueueByteConfirm[0]==0xCC && syncMachine.QueueByteConfirm[1]==0x00){
                ;
            }

            //lock motor
            if(syncMachine.QueueByteConfirm[0]==0x00 && syncMachine.QueueByteConfirm[1]==0x00){
                stepperX.stop();
                stepperX.stop();
                stepperX.stop();
            }
        }
    }
}

void CtrManager::HandllerManager() {
    stepperX.run();
    stepperY.run();
    stepperZ.run();
    HandllerPull();
    HandllerPush();
    HandllerNotify();
    HandllerHardware();
    _ProcessFunc();
}

