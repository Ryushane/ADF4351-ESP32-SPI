#include <SPI.h>
#include "ADF4351.h"

#include <Arduino.h>

#define CLOCKSPEED 1000000
#define CS 5
#define MOSI 23
#define SCLK 18
#define CE 25

ADF4351 PLL(SCLK, MOSI, CS, CE); // declares object PLL of type ADF4351. Will initialize it below.

void setup(){
    Serial.begin(115200);
    delay(500); // give it a sec to warm up
    SPI.begin();          // for communicating with DDS/PLLs

    PLL.initialize(297.7, 100); // initialize the PLL to output 400 Mhz, using an
                            // onboard reference of 10Mhz
}

void loop(){
    ;
}