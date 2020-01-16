/*
 * File:   22LC64.c
 * Author: kater
 *
 * Created on 11. ?íjna 2019, 17:52
 */

#include "PICF18LF46K22_ConfigSFR.h"
#include "24LC64_PIC.h"

char DataOUT[PAGE_SIZE]={};
char DataIN[PAGE_SIZE]={};


void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    //LEDs on port for show result data
    ANSELD=0;
    TRISD=0;

    Init_24LC64();
}

void ClearDevice(){
    PORTD=0x00;
    
    for(char i=0;i<PAGE_SIZE;i++){
        DataOUT[i]=i;
        DataIN[i]=0;
    }
}


void main(void) {
    InitDevice();
    ClearDevice();

    InitDevice();
    
    WriteTo_24LC64(0x01, 0x0F, 0xF0);
    LATD=ReadFrom_24LC64(0x01, 0x0F);
    NOP();
    delay_ms(1000);
    LATD=0;
    NOP();
    
    for(int i=0;i<H_MAX;i++){
        WriteTo_24LC64(i, i, i);
    }
    NOP();
    for(int i=0;i<H_MAX;i++){
        LATD=ReadFrom_24LC64(i, i);
        __delay_ms(100);
    }
    NOP();
    
    WritePage_24LC64(0x00, 0x00,DataOUT,PAGE_SIZE);
    delay_ms(10);
    SequentialRead_24LC64(0x00,0x00,DataIN,PAGE_SIZE);
    NOP();
    for(char i=0;i<PAGE_SIZE;i++){
        LATD=DataIN[i];
        __delay_ms(100);
    }

    LATD|=0x01;
    NOP();
    while(1);
    return;
}

/*void interrupt IRS(void){
}*/
