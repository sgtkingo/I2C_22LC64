/*
 * File:   22LC64.c
 * Author: kater
 *
 * Created on 11. ?íjna 2019, 17:52
 */

#include "PICF18LF46K22_ConfigSFR.h"
#include "I2CLibV2.h"

// address_of_24LC64_in the BUS I2C, W	
#define	ADR_SAA1064_W 0b01110000

void Init(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    //LEDs on port for show result data
    ANSELD=0;
    TRISD=0;
    
    I2C_INIT();
    I2C_CLEARBUS();
}

void ClearDevice(){
    PORTD=0x00;
}

void WriteTo_SAA1064(char data, char subaddr){
    I2C_STR();
    WriteI2C(ADR_SAA1064_W);
    WriteI2C(subaddr);
    //WriteI2C(0b01111010);
    WriteI2C(data);
    I2C_STP();
}


void main(void) {
    Init();
    ClearDevice();   
    
    char DATA=0x55;    
    WriteTo_SAA1064(DATA, 0x00);
    __delay_ms(10);
    
    while(1){
        asm("NOP");
    }
}