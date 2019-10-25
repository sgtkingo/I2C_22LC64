/*
 * File:   22LC64.c
 * Author: kater
 *
 * Created on 11. ?íjna 2019, 17:52
 */

#include "PICF18LF46K22_ConfigSFR.h"
#include "I2CLibV2.h"

// address_of_24LC64_in the BUS I2C, W	
#define	ADR_24LC64_W 0b10100000	
// address_of_24LC64_in the BUS I2C, R	
#define	ADR_24LC64_R 0b10100001

void Init(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    //LEDs on port for show result data
    ANSELD=0;
    TRISD=0;
}

void ClearDevice(){
    PORTD=0x00;
}

void WriteToSlave(char mem_H, char mem_L, char data){
    I2C_STR();
    WriteI2C(ADR_24LC64_W);
    WriteI2C(mem_H);
    WriteI2C(mem_L);
    WriteI2C(data);
    I2C_STP();
}

int ReadFromSlave(char mem_H, char mem_L){
    char data=0;
    
    I2C_STR();
    WriteI2C(ADR_24LC64_W);
    WriteI2C(mem_H);
    WriteI2C(mem_L);
    
    I2C_STR();
    WriteI2C(ADR_24LC64_R);
    data=I2CRead(false);
    I2C_STP();
    
    return data;
}

void main(void) {
    Init();
    ClearDevice();
    
    char mem_HB=0x00;
    char mem_LB=0x0F;
    char DATA=0x00;
    
    I2C_INIT();
    I2C_CLEARBUS();
    
    PORTD=ReadFromSlave(mem_HB,mem_LB);
    __delay_ms(5000);
    WriteToSlave(mem_HB,mem_LB,0xAA);
    __delay_ms(10);
    PORTD=0x00;
    
    while(1){
        /*if(DATA < 0xFF){
            WriteToSlave(mem_HB,mem_LB,++DATA);
            __delay_ms(10);
            PORTD=ReadFromSlave(mem_HB,mem_LB);
            __delay_ms(1000);
        }*/
        asm("NOP");
    }
}

/*void interrupt IRS(void){
    if(PIR1bits.SSP1IF){
        PIR1bits.SSP1IF=0;
    }

}*/
