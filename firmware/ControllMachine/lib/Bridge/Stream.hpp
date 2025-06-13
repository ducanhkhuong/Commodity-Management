#include <Arduino.h>
#include <stdint.h>
#include "config.h"

class BridgeStream {
public:
    void begin(uint32_t baud_rate);
    void read();
    void write(uint8_t* data, size_t length);  
    uint8_t* getData();
    size_t length() const;

private:
    uint8_t data[BUFFER_SIZE];
    size_t bytesRead = 0;
    void debug(uint8_t* data, size_t length);
};
