#include <avr/io.h>
#include <util/delay.h>
#include "CtrManager.hpp"

int main() {
    CtrManager ctrManager(BAUD_RATE);
    while(1){
        ctrManager.HandllerManager(); 
    }
}