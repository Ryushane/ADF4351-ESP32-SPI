#include <SPI.h>
#include "ADF4351.h"

#include <Arduino.h>

#define CLOCKSPEED 1000000
#define CS 5
#define MOSI 23
#define SCLK 18
#define CE 25

ADF4351 PLL(CS); // declares object PLL of type ADF4351. Will initialize it below.

void setup(){
    pinMode(CS, OUTPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(SCLK, OUTPUT);
    pinMode(CE, OUTPUT);

    digitalWrite(CS, HIGH);
    digitalWrite(CE, HIGH);
    delay(500); // give it a sec to warm up
    SPI.begin();          // for communicating with DDS/PLLs
    SPI.beginTransaction(SPISettings(CLOCKSPEED, MSBFIRST, SPI_MODE0));

    PLL.initialize(281, 25); // initialize the PLL to output 400 Mhz, using an
                            // onboard reference of 10Mhz
    SPI.endTransaction();
}

void loop(){
    ;
}