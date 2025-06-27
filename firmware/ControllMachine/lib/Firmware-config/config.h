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

#define TIMER_TICK_HANDLER 500          
#define TIMER_TICK_CALLBACK_RESPONSE 100
#define TIMER_TICK_CALLBACK_CHECK_HW 50

#define BUTTON_AXIS_X PC0 //A0
#define BUTTON_AXIS_Y PC1 //A1
#define BUTTON_AXIS_Z PC2 //A2

#define LED_TEST PB5//13

#define MOTOR_STEP_X 2
#define MOTOR_STEP_Y 3
#define MOTOR_STEP_Z 4

#define MOTOR_DIR_X 5
#define MOTOR_DIR_Y 6
#define MOTOR_DIR_Z 7

#define MOTOR_ENA 8 


//Lựa chọn cấu hình 
// DEBUG : bật lên 1 để sử dụng debug qua stream hiển thị log 
//         đưa về 0 để về trạng thái sử dụng bình thường  
// BUFFER_MODE  : bật 1 để dùng 10 byte dữ liệu 
//                bật 2 để dùng 30 byte dữ liệu 
//                bật 3 để dùng 50 byte dữ liệu   

// //config
// byte[] modehomeConfig    = new byte[] { 0x1F, 0xFF};
// byte[] modesendConfig    = new byte[] { 0x1F, 0xAA};
// byte[] moderecieveConfig = new byte[] { 0x1F, 0xBB};
// byte[] modetestConfig    = new byte[] { 0x1F, 0xCC};
// //controll
// byte[] index1SendControll = new byte[] { 0x2F, 0x01, 0x1F, 0xAA};
// byte[] index2SendControll = new byte[] { 0x2F, 0x02, 0x1F, 0xAA};
// byte[] index3SendControll = new byte[] { 0x2F, 0x03, 0x1F, 0xAA};
// byte[] index4SendControll = new byte[] { 0x2F, 0x04, 0x1F, 0xAA};

// byte[] index1RecvControll = new byte[] { 0x2F, 0x01, 0x1F, 0xBB};
// byte[] index2RecvControll = new byte[] { 0x2F, 0x02, 0x1F, 0xBB};
// byte[] index3RecvControll = new byte[] { 0x2F, 0x03, 0x1F, 0xBB};
// byte[] index4RecvControll = new byte[] { 0x2F, 0x04, 0x1F, 0xBB};
// //response
// byte[] modeResponse = new byte[] {0x3F,0x00,0x00,0x00,0x00,0x00,0x00};