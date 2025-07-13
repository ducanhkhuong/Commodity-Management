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

#define BUTTON_AXIS_X PE4 //2
#define BUTTON_AXIS_Y PJ1 //14
#define BUTTON_AXIS_Z PD3 //18