#include "Stream.hpp"
#include <HardwareSerial.h>
#include <string.h>
#include <stdio.h>

void BridgeStream::begin(uint32_t baudRate) {
    Serial.begin(baudRate);
}

void BridgeStream::read() {
    bytesRead = 0;
    if (Serial.available() > 0) {
        memset(data, 0, sizeof(data));
        bytesRead = Serial.readBytes(data, sizeof(data) - 1);
        data[bytesRead] = '\0';
        #if DEBUG == 1
        debug(data, bytesRead);
        #endif
    }
}

void BridgeStream::write(uint8_t* data, size_t length) {
    debug(data, length);
}

void BridgeStream::debug(uint8_t* data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (data[i] < 0x10) Serial.print("0");
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

uint8_t* BridgeStream::getData() {
    return data;
}

size_t BridgeStream::length() const {
    return bytesRead;
}
