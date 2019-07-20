/* 
   ADF4351.cpp - ADF4351 PLL Communication Library
   Created by Neal Pisenti, 2013.
   JQI - Strontium - UMD

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   aunsigned long with this program.  If not, see <http://www.gnu.org/licenses/>.


 */

#include "Arduino.h"
#include "SPI.h"
#include "ADF4351.h"



// NOTE: Currently, set up for channel spacing of 10Mhz; might ammend later if needed,
//  but this was easy and takes care of forseeable applications...
//  This means we'll be operating in int-N mode.
//  
//  Currently, keep frequency between 140Mhz and 3Ghz.

/* CONSTRUCTOR */

// Constructor function; initializes communication pinouts

ADF4351::ADF4351(byte ssPin) {
    _ssPin = ssPin;
    pinMode(_ssPin, OUTPUT);
    digitalWrite(_ssPin, HIGH);
}





// Initializes a new ADF4351 object, with refClk (in Mhz), and initial frequency.
void ADF4351::initialize(int freq, int refClk = 10){
    _refClk = refClk;
    _phase = 1;

    _feedbackType = 1;
    _powerdown = 0;
    _auxEnabled = 0;
    _rfEnabled = 1;

    // default power = 5dbm
    _auxPower = 0;
    _rfPower = 3;

    // sets register values which don't have dynamic values...
    ADF4351::setR1();
    ADF4351::setR3();
    ADF4351::setR5();


    ADF4351::setFreq(freq);
//    ADF4351::update();
}

// gets current frequency setting
int ADF4351::getFreq(){
    return _freq;
}

void ADF4351::setFreq(int freq){
    _freq = freq;
    int multiplier;

    // selects the right divider range (ie, output divider is 2^(_divider))
    // the multiplier is required if you're using fundamental feedback to N-counter
    if (_freq <= 68 && _freq >= 35){
        _divider = 6;
        multiplier = 64;
    } else if (_freq >= 69 && _freq <= 137){
        _divider = 5;
        multiplier = 32;
    } else if (_freq >= 138 && _freq <= 274){
        _divider = 4;
        multiplier = 16;
    } else if (_freq >= 275 && _freq <= 549){
        _divider = 3;
        multiplier = 8;
    } else if (_freq >= 550 && _freq <= 1099){
        _divider = 2;
        multiplier = 4;
    } else if (_freq >= 1100 && _freq <= 2199){
        _divider = 1;
        multiplier = 2;
    } else if (_freq >= 2200 && _freq <= 4400){
        _divider = 0;
        multiplier = 1;
    } else{
        _divider = 0;
        multiplier = 1;
    }

    if (_feedbackType == 0){
        _int = _freq/_refClk;
    } else{
        _int = _freq*multiplier/_refClk;
    }
    ADF4351::update();
}



// updates dynamic registers, and writes values to PLL board
void ADF4351::update(){
    // updates registers with dynamic values...
    ADF4351::setR0();
    ADF4351::setR2();
    ADF4351::setR4();

    // writes registers to device
    ADF4351::writeRegister(_r5);
    ADF4351::writeRegister(_r4);
    ADF4351::writeRegister(_r3);
    ADF4351::writeRegister(_r2);
    ADF4351::writeRegister(_r1);
    ADF4351::writeRegister(_r0);
}

void ADF4351::setFeedbackType(bool feedback){
    _feedbackType = feedback;
}

void ADF4351::powerDown(bool pd){
    _powerdown = pd;
    ADF4351::setR2();
    ADF4351::update();

}

void ADF4351::rfEnable(bool rf){
    _rfEnabled = rf;
    ADF4351::setR4();
    ADF4351::update();
}

// CAREFUL!!!! pow must be 0, 1, 2, or 3... corresponding to -4, -1, 3, 5 dbm.
void ADF4351::setRfPower(int pow){
    _rfPower = pow;
    ADF4351::setR4();
    ADF4351::update();
}

void ADF4351::auxEnable(bool aux){
    _auxEnabled = aux;
    ADF4351::setR4();
    ADF4351::update();
}

// CAREFUL!!!! pow must be 0, 1, 2, or 3... corresponding to -4, -1, 3, 5 dbm.
void ADF4351::setAuxPower(int pow){
    _auxPower = pow;
    ADF4351::setR4();
    ADF4351::update();
}

// REGISTER UPDATE FUNCTIONS

void ADF4351::setR0(){
    unsigned long r0 = (_int << 15); // sets int value... 
    byte r0Ary[] = { lowByte(r0 >> 24), lowByte(r0 >> 16), lowByte(r0 >> 8), lowByte(r0) };
    memcpy(&_r0, &r0Ary, sizeof(r0Ary));
}

void ADF4351::setR1(){
    unsigned long r1 =   (_phase << 15) + // phase value = 1
            (2 << 3) + // modulus value = 1
             1; // register value
    byte r1Ary[] = { lowByte(r1 >> 24), lowByte(r1 >> 16), lowByte(r1 >> 8), lowByte(r1) };
    memcpy(&_r1, &r1Ary, sizeof(r1Ary));
}

void ADF4351::setR2(){
    unsigned long r2 =   (0 << 26) +  // muxout three state
            (1 << 14) +  // r-counter = 1 with 100M oscillator r-counter = 4
            (7 << 9) +  // charge pump = 2.5
            (5 << 6) +  // digital lock detect + polarity
            (_powerdown << 5) +   // powerdown 0 = false; 1 = true
            2; // register value
    
    byte r2Ary[] =  { lowByte(r2 >> 24), lowByte(r2 >> 16), lowByte(r2 >> 8), lowByte(r2) };
    memcpy(&_r2, &r2Ary, sizeof(r2Ary));
}

void ADF4351::setR3(){
   unsigned long r3 = 1203‬; // (all zero, except register control value = 3); 4B3
   byte r3Ary[] = { lowByte(r3 >> 24), lowByte(r3 >> 16), lowByte(r3 >> 8), lowByte(r3) };
   memcpy(&_r3, &r3Ary, sizeof(r3Ary));
}

void ADF4351::setR4(){

    unsigned long r4 = (_feedbackType << 23) + // divided/fundamental feedback
        (_divider << 20) + // RF divider
        (200 << 12) + // band select clock divider
        (0 << 9) + // vco powerdown = false; MTLD = 1; aux output = divided;
        (_auxEnabled << 8) + // AUX OUTPUT enable/disable
        (_auxPower << 6) + // aux output power = {-4, -1, 2, 5dbm}
        (_rfEnabled << 5) + // RF OUTPUT ENABLED
        (_rfPower << 3) + // RF output power = 5dbm
        4;  // register select

   byte r4Ary[] = { lowByte(r4 >> 24), lowByte(r4 >> 16), lowByte(r4 >> 8), lowByte(r4) };
   memcpy(&_r4, &r4Ary, sizeof(r4Ary));


}

void ADF4351::setR5(){

    unsigned long r5 = (1 << 22) + (3<<19) + 5; // lock detect pin mode = digital lock detect
   byte r5Ary[] = { lowByte(r5 >> 24), lowByte(r5 >> 16), lowByte(r5 >> 8), lowByte(r5) };
   memcpy(&_r5, &r5Ary, sizeof(r5Ary));

}
// Writes SPI to particular register.
//      registerInfo is a 2-element array which contains [register, number of bytes]
void ADF4351::writeRegister(byte data[]){


    digitalWrite(_ssPin, LOW);

    // Writes the data
    for(int i = 0; i < 4 ; i++){
        SPI.transfer(data[i]);
    }

    digitalWrite(_ssPin, HIGH);

}

