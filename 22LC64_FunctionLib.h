/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/*  
 * Author: Jiri Konecny 
 * Comments: EEPROM 22LC64 function header for PIC
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef EEPROM22LC64_FUNCTIONLIB_PIC
#define EEPROM22LC64_FUNCTIONLIB_PIC

#include "I2CLibV2.h"

// address_of_24LC64_in the BUS I2C, W	
#define	ADR_24LC64_W 0b10100000	
// address_of_24LC64_in the BUS I2C, R	
#define	ADR_24LC64_R 0b10100001

void Init_22LC64();
void WriteTo_22LC64(char mem_H, char mem_L, char data);
int ReadFrom_22LC64(char mem_H, char mem_L);

void Init_22LC64(){
    I2C_INIT();
    I2C_CLEARBUS();
}

void WriteTo_22LC64(char mem_H, char mem_L, char data){
    I2C_STR();
    WriteI2C(ADR_24LC64_W);
    WriteI2C(mem_H);
    WriteI2C(mem_L);
    WriteI2C(data);
    I2C_STP();
}

int ReadFrom_22LC64(char mem_H, char mem_L){
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

#endif	/* EEPROM22LC64_FUNCTIONLIB_PIC */

