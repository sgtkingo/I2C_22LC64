/*
 * File:   22LC64.c
 * Author: kater
 *
 * Created on 11. ?íjna 2019, 17:52
 */

#include "PICF18LF46K22_ConfigSFR.h"
#include "MCP4726_DA.h"


void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    //LEDs on port for show result data
    ANSELD=0;
    TRISD=0;
    
    Init_MCP4726(default_adr_MCP4726);
}

void ClearDevice(){
    PORTD=0x00;
}


void main(void) {
    InitDevice();
    ClearDevice();
    
    unsigned int DATA_VOLTAGE=0;
    
    Set_VoltageAsData_MCP4726(DATA_VOLTAGE);
    __delay_ms(1);
    ReadFrom_MCP4726();
    
    for(int i=0;i<EEPROM_MEM_SIZE_4726;i++){
        PORTD=EEPROM_MEM_4726[i];
        __delay_ms(250);
    }
    PORTD=0xFF;
    while(1){
        asm("NOP");
    }
}

/*void interrupt IRS(void){
}*/
