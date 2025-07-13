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
    HandlerHomeTimer.start();
    ProcessFoward.start(); 

    //config
    stepperX.setMaxSpeed(12000);
    stepperX.setAcceleration(7000);
    stepperX.setCurrentPosition(0);

    stepperY.setMaxSpeed(12000);
    stepperY.setAcceleration(7000);
    stepperY.setCurrentPosition(0);

    stepperZ.setMaxSpeed(12000);
    stepperZ.setAcceleration(7000);
    stepperZ.setCurrentPosition(0);

    stepperX.stop();
    stepperX.stop();
    stepperX.stop();

    stepperMonitor.stepStage = 0;
    stepperMonitor.moving = false;
    stepperMonitor.homeFinished = false;
    hwStatus.buttonX = false;
    hwStatus.buttonY = false;
    hwStatus.buttonZ = false;
}

void CtrManager::HandllerPull() {
    if(HandlerUartRecieveTimer.isRunning()){
        if (HandlerUartRecieveTimer.isCheckTime(TIMER_TICK_HANDLER_RECV)){
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
        if (HandlerUartTransmitTimer.isCheckTime(TIMER_TICK_HANDLER_SEND)) {
            stream.write(hwStatus.bufferPush, sizeof(hwStatus.bufferPush));
        }
    }
}



void CtrManager::HandllerNotify() {
    if(HandlerNotifyTimer.isRunning()){
        if(HandlerNotifyTimer.isCheckTime(TIMER_TICK_HANDLER_RECV)){
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

void CtrManager::HandllerHome(){
    if (!HandlerHomeTimer.isRunning()) return;
    if (!HandlerHomeTimer.isCheckTime(TIMER_TICK_HANDLER_EXECUTE)) return;
    if (stepperMonitor.stepStage == 4 && !stepperMonitor.homeFinished
        && !stepperX.isRunning() && !stepperY.isRunning() && !stepperZ.isRunning() 
        && hwStatus.buttonX && hwStatus.buttonY && hwStatus.buttonZ
        ) 
    {
        stepperY.moveTo(8  * STEPS_PER_MM);
        stepperZ.moveTo(-120 * STEPS_PER_MM);
        stepperX.moveTo(-45 * STEPS_PER_MM);
        stepperMonitor.moving = true;
        stepperMonitor.homeFinished = true;
    }

    if (stepperMonitor.stepStage == 4 && stepperMonitor.homeFinished
        && stepperMonitor.moving
        && !stepperX.isRunning() && !stepperY.isRunning() && !stepperZ.isRunning()
        ) 
    {
        stepperMonitor.moving = false;
        stepperMonitor.stepStage = 5;
        stepperMonitor.homeFinished = false;
        stepperX.setCurrentPosition(0);
        stepperY.setCurrentPosition(0);
        stepperZ.setCurrentPosition(0);
        stepperX.stop(); 
        stepperY.stop(); 
        stepperZ.stop();
    }
}


void CtrManager::_ProcessFunc() {
    static uint8_t lastCmd0 = 0;
    static uint8_t lastCmd1 = 0;
    if(ProcessFoward.isRunning()){
        if(ProcessFoward.isCheckTime(TIMER_TICK_HANDLER_EXECUTE)){
            //home
            if (syncMachine.QueueByteConfirm[0] == 0xFF && syncMachine.QueueByteConfirm[1] == 0x00) {
                if (lastCmd0 != syncMachine.QueueByteConfirm[0] || lastCmd1 != syncMachine.QueueByteConfirm[1]) {
                    stepperMonitor.stepStage = 0;
                    stepperMonitor.moving = false;
                    lastCmd0 = syncMachine.QueueByteConfirm[0];
                    lastCmd1 = syncMachine.QueueByteConfirm[1];
                }

                switch (stepperMonitor.stepStage) {
                    case 0:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(-10000 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        } else if (stepperMonitor.moving && hwStatus.buttonY && !hwStatus.buttonX && !hwStatus.buttonZ) {
                            stepperY.setCurrentPosition(0);
                            stepperMonitor.stepStage = 1;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 1:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(10000 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        } else if (stepperMonitor.moving && hwStatus.buttonZ && hwStatus.buttonY && !hwStatus.buttonX) {
                            stepperZ.setCurrentPosition(0);
                            stepperMonitor.stepStage = 2;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 2:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(10000 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        } else if (stepperMonitor.moving && hwStatus.buttonX && hwStatus.buttonY && hwStatus.buttonZ) {
                            stepperX.setCurrentPosition(0);
                            stepperMonitor.stepStage = 3; 
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 3:
                        if(hwStatus.buttonX && hwStatus.buttonY && hwStatus.buttonZ && stepperMonitor.stepStage == 3){
                            stepperMonitor.stepStage = 4;
                        }
                    break;
                }
            }

            //send
            if(syncMachine.QueueByteConfirm[0]==0xAA && syncMachine.QueueByteConfirm[1]==0x01){
                if (lastCmd0 != syncMachine.QueueByteConfirm[0] || lastCmd1 != syncMachine.QueueByteConfirm[1]) {
                    stepperMonitor.stepStage = 0;
                    stepperMonitor.moving = false;
                    lastCmd0 = syncMachine.QueueByteConfirm[0];
                    lastCmd1 = syncMachine.QueueByteConfirm[1];
                }
                switch (stepperMonitor.stepStage) {
                    case 0:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 1;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 1:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(110 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 2;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 2:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 3;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 3:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(-375 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 4;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 4:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 5;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 5:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 6;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 6:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 7;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 7:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 8;
                            stepperMonitor.moving = false;
                        }
                    break;
                    
                    case 8:             
                        stepperMonitor.moving = false;        
                        stepperX.stop();
                        stepperY.stop();
                        stepperZ.stop();
                    break;
                }
            }
            if(syncMachine.QueueByteConfirm[0]==0xAA && syncMachine.QueueByteConfirm[1]==0x02){
                if (lastCmd0 != 0xAA || lastCmd1 != 0x02) {
                    stepperMonitor.stepStage = 0;
                    stepperMonitor.moving = false;
                    lastCmd0 = 0xAA;
                    lastCmd1 = 0x02;
                }
                switch (stepperMonitor.stepStage) {
                    case 0:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 1;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 1:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(110 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 2;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 2:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 3;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 3:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(-235 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 4;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 4:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 5;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 5:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 6;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 6:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 7;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 7:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 8;
                            stepperMonitor.moving = false;
                        }
                    break;
                    
                    case 8:             
                        stepperMonitor.moving = false;        
                        stepperX.stop();
                        stepperY.stop();
                        stepperZ.stop();
                    break;
                }
            }
            if(syncMachine.QueueByteConfirm[0]==0xAA && syncMachine.QueueByteConfirm[1]==0x03){
                if (lastCmd0 != 0xAA || lastCmd1 != 0x03) {
                    stepperMonitor.stepStage = 0;
                    stepperMonitor.moving = false;
                    lastCmd0 = 0xAA;
                    lastCmd1 = 0x03;
                }
                switch (stepperMonitor.stepStage) {
                    case 0:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 1;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 1:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(110 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 2;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 2:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 3;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 3:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(-120 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 4;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 4:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 5;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 5:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 6;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 6:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(-372 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 7;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 7:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 8;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 8:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-137 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 9;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 9:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 10;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 10:
                    if (!stepperMonitor.moving) {
                            stepperX.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 11;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 11:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 12;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 12:
                        stepperMonitor.moving = false;        
                        stepperX.stop();
                        stepperY.stop();
                        stepperZ.stop();
                    break;
                }
            }
            if(syncMachine.QueueByteConfirm[0]==0xAA && syncMachine.QueueByteConfirm[1]==0x04){
                if (lastCmd0 != 0xAA || lastCmd1 != 0x04) {
                    stepperMonitor.stepStage = 0;
                    stepperMonitor.moving = false;
                    lastCmd0 = 0xAA;
                    lastCmd1 = 0x04;
                }
                switch (stepperMonitor.stepStage) {
                    case 0:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 1;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 1:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(110 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 2;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 2:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 3;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 3:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(-120 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 4;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 4:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 5;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 5:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 6;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 6:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(-237 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 7;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 7:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 8;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 8:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-137 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 9;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 9:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 10;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 10:
                    if (!stepperMonitor.moving) {
                            stepperX.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 11;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 11:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 12;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 12:
                        stepperMonitor.moving = false;        
                        stepperX.stop();
                        stepperY.stop();
                        stepperZ.stop();
                    break;
                }
            }
            
            //recv
            if(syncMachine.QueueByteConfirm[0]==0xBB && syncMachine.QueueByteConfirm[1]==0x01){
                if (lastCmd0 != syncMachine.QueueByteConfirm[0] || lastCmd1 != syncMachine.QueueByteConfirm[1]) {
                    stepperMonitor.stepStage = 0;
                    stepperMonitor.moving = false;
                    lastCmd0 = syncMachine.QueueByteConfirm[0];
                    lastCmd1 = syncMachine.QueueByteConfirm[1];
                }
                switch (stepperMonitor.stepStage) {
                    case 0:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(-375 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 1;
                            stepperMonitor.moving = false;
                        }
                    break;
                    
                    case 1:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 2;
                            stepperMonitor.moving = false;
                        }
                    break;
                
                    case 2:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(110 * STEPS_PER_MM);
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
                            stepperX.moveTo(-120 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 5;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 5:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 6;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 6:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 7;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 7:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 8;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 8:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 9;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 9:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-137 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 10;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 10:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 11;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 11:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 12;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 12:
                        stepperMonitor.moving = false;        
                        stepperX.stop();
                        stepperY.stop();
                        stepperZ.stop();
                    break;
                }
            }
            if(syncMachine.QueueByteConfirm[0]==0xBB && syncMachine.QueueByteConfirm[1]==0x02){
                if (lastCmd0 != syncMachine.QueueByteConfirm[0] || lastCmd1 != syncMachine.QueueByteConfirm[1]) {
                    stepperMonitor.stepStage = 0;
                    stepperMonitor.moving = false;
                    lastCmd0 = syncMachine.QueueByteConfirm[0];
                    lastCmd1 = syncMachine.QueueByteConfirm[1];
                }
                switch (stepperMonitor.stepStage) {
                    case 0:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(-235 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 1;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 1:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 2;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 2:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(110 * STEPS_PER_MM);
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
                            stepperX.moveTo(-120 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 5;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 5:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 6;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 6:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 7;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 7:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 8;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 8:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 9;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 9:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-137 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 10;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 10:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 11;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 11:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 12;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 12:
                        stepperMonitor.moving = false;        
                        stepperX.stop();
                        stepperY.stop();
                        stepperZ.stop();
                    break;
                }
            }
            if(syncMachine.QueueByteConfirm[0]==0xBB && syncMachine.QueueByteConfirm[1]==0x03){
                if (lastCmd0 != syncMachine.QueueByteConfirm[0] || lastCmd1 != syncMachine.QueueByteConfirm[1]) {
                    stepperMonitor.stepStage = 0;
                    stepperMonitor.moving = false;
                    lastCmd0 = syncMachine.QueueByteConfirm[0];
                    lastCmd1 = syncMachine.QueueByteConfirm[1];
                }
                switch (stepperMonitor.stepStage) {
                    case 0:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-137 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 1;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 1:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(-372 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 2;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 2:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 3;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 3:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 4;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 4:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 5;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 5:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-40 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 6;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 6:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(-120 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 7;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 7:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
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
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 10;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 10:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-137 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 11;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 11:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 12;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 12:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 13;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 13:
                        stepperMonitor.moving = false;        
                        stepperX.stop();
                        stepperY.stop();
                        stepperZ.stop();
                    break;
                }
            }
            if(syncMachine.QueueByteConfirm[0]==0xBB && syncMachine.QueueByteConfirm[1]==0x04){
                if (lastCmd0 != syncMachine.QueueByteConfirm[0] || lastCmd1 != syncMachine.QueueByteConfirm[1]) {
                    stepperMonitor.stepStage = 0;
                    stepperMonitor.moving = false;
                    lastCmd0 = syncMachine.QueueByteConfirm[0];
                    lastCmd1 = syncMachine.QueueByteConfirm[1];
                }
                switch (stepperMonitor.stepStage) {
                    case 0:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-137 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 1;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 1:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(-237 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 2;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 2:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 3;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 3:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 4;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 4:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 5;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 5:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-40 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 6;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 6:
                        if (!stepperMonitor.moving) {
                            stepperX.moveTo(-120 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperX.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 7;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 7:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
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
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(80 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 10;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 10:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(-137 * STEPS_PER_MM);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 11;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 11:
                        if (!stepperMonitor.moving) {
                            stepperY.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperY.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 12;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 12:
                        if (!stepperMonitor.moving) {
                            stepperZ.moveTo(0);
                            stepperMonitor.moving = true;
                        }
                        if (stepperMonitor.moving && stepperZ.distanceToGo() == 0) {
                            stepperMonitor.stepStage = 13;
                            stepperMonitor.moving = false;
                        }
                    break;

                    case 13:
                        stepperMonitor.moving = false;        
                        stepperX.stop();
                        stepperY.stop();
                        stepperZ.stop();
                    break;
                }
            }

            //stop
            if(syncMachine.QueueByteConfirm[0]==0xCC && syncMachine.QueueByteConfirm[1]==0x00){
                stepperX.setCurrentPosition(0);
                stepperY.setCurrentPosition(0);
                stepperZ.setCurrentPosition(0);
                stepperX.stop(); 
                stepperY.stop(); 
                stepperZ.stop();
            }

            //lock motor
            if(syncMachine.QueueByteConfirm[0]==0x00 && syncMachine.QueueByteConfirm[1]==0x00){
                stepperX.setCurrentPosition(0);
                stepperY.setCurrentPosition(0);
                stepperZ.setCurrentPosition(0);
                stepperX.stop();
                stepperY.stop();
                stepperZ.stop();
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
    HandllerHome();
    _ProcessFunc();
}

