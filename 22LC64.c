/*
 * File:   22LC64.c
 * Author: kater
 *
 * Created on 11. ?íjna 2019, 17:52
 */

#include "PICF18LF46K22_ConfigSFR.h"
#include "22LC64_FunctionLib.h"


void Init(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    //LEDs on port for show result data
    ANSELD=0;
    TRISD=0;
    
    Init_22LC64();
}

void ClearDevice(){
    PORTD=0x00;
}


void main(void) {
    Init();
    ClearDevice();
    
    char mem_HB=0x00;
    char mem_LB=0x0F;
    char DATA=0x55;
    
    PORTD=ReadFrom_22LC64(mem_HB,mem_LB);
    __delay_ms(10);
    WriteTo_22LC64(mem_HB,mem_LB,DATA);
    __delay_ms(10);
    
    while(1){
        asm("NOP");
    }
}

/*void interrupt IRS(void){
}*/
