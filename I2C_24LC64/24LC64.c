/*
 * File:   24LC64.c
 * Author: kater
 *
 * Created on 23. ledna 2020, 11:58
 */

#include "PICF18LF46K22_ConfigSFR.h"
#include "24LC64_PIC.h"


char DataOUT[PAGE_SIZE]={};
char DataIN[PAGE_SIZE]={};

void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH  
    ANSELD=0x00;
    TRISD=0x00;
    
    Init_24LC64();
}

void ClearDevice(){
    for(char i=0;i<PAGE_SIZE;i++){
        DataOUT[i]=i;
        DataIN[i]=0;
    }
    PORTD=0x00;
}


void main(void) {
    InitDevice();
    ClearDevice();
    
    WriteTo_24LC64(0x0F, 0x0F, 0xAA);
    delay_ms(10);
    LATD=ReadFrom_24LC64(0x0F, 0x0F);
    NOP();
    delay_ms(1000);
    
    for(int i=0;i<H_MAX;i++){
        WriteTo_24LC64(i, i, i);
    }
    NOP();
    for(int i=0;i<H_MAX;i++){
        LATD=ReadFrom_24LC64(i, i);
        delay_ms(100);
    }
    
    WritePage_24LC64(0x00, 0x00,DataOUT,PAGE_SIZE);
    delay_ms(10);
    SequentialRead_24LC64(0x00,0x00,DataIN,PAGE_SIZE);
    NOP();
    for(char i=0;i<PAGE_SIZE;i++){
        LATD=DataIN[i];
        delay_ms(100);
    }
    delay_ms(1000);
    LATD=0x00;
    
    while(1);
    return;
}
