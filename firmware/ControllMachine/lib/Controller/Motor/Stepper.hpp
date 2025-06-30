#include <AccelStepper.h>

#define MOTOR_STEP_X A0
#define MOTOR_STEP_Y A6
#define MOTOR_STEP_Z 46

#define MOTOR_DIR_X  A1
#define MOTOR_DIR_Y  A7
#define MOTOR_DIR_Z  48

#define MOTOR_ENA_X  38
#define MOTOR_ENA_Y  A2
#define MOTOR_ENA_Z  A8

#define MOTOR_STEPS     200
#define MICROSTEPS      16
#define SCREW_LEAD_MM   8.0
#define STEPS_PER_MM    ((long)((MOTOR_STEPS * MICROSTEPS) / SCREW_LEAD_MM))

#define BUTTON_AXIS_X PE5 //3
#define BUTTON_AXIS_Y PJ1 //14
#define BUTTON_AXIS_Z PD3 //18

// long targetA_X = 0;
// long targetB_X = -120 * STEPS_PER_MM;
// long targetPos_X = targetB_X;
// bool movingX = false;
// unsigned long arriveX = 0;

// //max = 60
// //0=>60
// //0 đi vào 
// //60 đi ra 
// long targetA_Y = 0;
// long targetB_Y = 40 * STEPS_PER_MM;
// long targetPos_Y = targetB_Y;
// bool movingY = false;
// unsigned long arriveY = 0;

// //max = 120
// //0=> 120
// //-120 đi xuống 
// // 0 đi lên  
// long targetA_Z = 0;
// long targetB_Z = -60 * STEPS_PER_MM;
// long targetPos_Z = targetB_Z;
// bool movingZ = false;
// unsigned long arriveZ = 0;