/*
 * File:   22LC64.c
 * Author: kater
 *
 * Created on 11. ?íjna 2019, 17:52
 */

#include "PICF18LF46K22_ConfigSFR.h"
#include "24LC64_EEPROM.h"


void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    //LEDs on port for show result data
    ANSELD=0;
    TRISD=0;
    Init_24LC64(default_adr_24LC64);
}

void TestDevice(){
    return;
}

void ClearDevice(){
    PORTD=0x00;
}

#define N 5

void main(void) {
    InitDevice();
    ClearDevice();
    
    char mem_HB=0x01;
    char mem_LB=0x0F;
    char DATA=0xAA;
    
    WriteTo_24LC64(mem_HB,mem_LB,DATA);
    LATD=ReadFrom_24LC64(mem_HB,mem_LB);
    
    /*char InputDataArray[N]={0x00,0x55,0xA8,0xFF,0xEA};
    char OutputDataArray[N]={0x00,0x00,0x00,0x00,0x00};
    
    delay_ms(1000);
    for(char i=0;i<0xFF;i++){
        WriteTo_24LC64(0x00,i,i);
    }
    delay_ms(1000);
    for(char i=0;i<0xFF;i++){
        LATD=ReadFrom_24LC64(0x00,i);
        delay_ms(100);
    }
    NOP();
    PORTD=0xFF;
    delay_ms(1000);
    
    WritePage_24LC64(0x0F, 0x00,InputDataArray,N);
    SequentialRead_24LC64(0x0F, 0x00,OutputDataArray,N);
    PORTD=0x00;
    NOP();*/
    while(1){
        asm("NOP");
    }
}

/*void interrupt IRS(void){
}*/
