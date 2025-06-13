#define DEBUG 1
#define BUFFER_MODE 2
    #if BUFFER_MODE == 1
        #define BUFFER_SIZE 10
        #define BAUD_RATE 9600
    #elif BUFFER_MODE == 2
        #define BUFFER_SIZE 30
        #define BAUD_RATE 38400
    #elif BUFFER_MODE == 3
        #define BUFFER_SIZE 50
        #define BAUD_RATE 115200
    #endif

#define TIMER_TICK_HANDLER 1000          
#define TIMER_TICK_CALLBACK_CHECK_HW 200

#define BUTTON_AXIS_X PC0 //A0
#define BUTTON_AXIS_Y PC1 //A1
#define BUTTON_AXIS_Z PC2 //A2

#define MOTOR_STEP_X 1
#define MOTOR_STEP_Y 2
#define MOTOR_STEP_Z 3

#define MOTOR_DIR_X 4
#define MOTOR_DIR_Y 5
#define MOTOR_DIR_Z 6

#define MOTOR_ENA_X 7 
#define MOTOR_ENA_Y 8 
#define MOTOR_ENA_Z 9


//Lựa chọn cấu hình 
// DEBUG : bật lên 1 để sử dụng debug qua stream hiển thị log 
//         đưa về 0 để về trạng thái sử dụng bình thường  
// BUFFER_MODE  : bật 1 để dùng 10 byte dữ liệu 
//                bật 2 để dùng 30 byte dữ liệu 
//                bật 3 để dùng 50 byte dữ liệu   