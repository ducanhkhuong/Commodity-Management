#define DEBUG 1
#define BUFFER_MODE 2
    #if BUFFER_MODE == 1
        #define BUFFER_SIZE 10
        #define BAUD_RATE 9600
    #elif BUFFER_MODE == 2
        #define BUFFER_SIZE 30
        #define BAUD_RATE 115200
    #elif BUFFER_MODE == 3
        #define BUFFER_SIZE 50
        #define BAUD_RATE 115200
    #endif

#define TIMER_TICK_HANDLER           500          
#define TIMER_TICK_CALLBACK_RESPONSE 500
#define TIMER_TICK_CALLBACK_CHECK_HW 50
