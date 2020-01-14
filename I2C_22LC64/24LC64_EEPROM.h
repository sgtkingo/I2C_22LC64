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
 * Comments: EEPROM 22LC64 config header for PIC
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MC_26LC64_PIC
#define MC_26LC64_PIC

#include "I2C_PIC.h"
#define default_adr_24LC64 0b10100000

char ADR_24LC46=0;

// address_of_24LC64_in the BUS I2C, W	
#define	ADR_24LC64_W (ADR_24LC46 MASK_W)	
// address_of_24LC64_in the BUS I2C, R	
#define	ADR_24LC64_R (ADR_24LC46 MASK_R)

//Intit EEPROM, enter address or set to default
void Init_24LC64(char address);
void WriteTo_24LC64(char mem_H, char mem_L, char data);
int ReadFrom_24LC64(char mem_H, char mem_L);

void WritePage_24LC64(char mem_H, char mem_L,char* data,unsigned int N);
void OpenWriteStream_24LC64(char mem_H, char mem_L);
void WriteStream_24LC64(char data);

void OpenReadStream_24LC64(char mem_H, char mem_L);
void SequentialRead_24LC64(char mem_H, char mem_L,char *data, unsigned int N);

void CloseStream_24LC64();

void Init_24LC64(char address){
    ADR_24LC46=address;
    I2C_INIT();
    I2C_CLEARBUS();
}

void WriteTo_24LC64(char mem_H, char mem_L, char data){
    I2C_STR();
    WriteI2C(ADR_24LC64_W);
    WriteI2C(mem_H);
    WriteI2C(mem_L);
    WriteI2C(data);
    I2C_STP();
    I2C_PAUSE(5000);
}

void OpenWriteStream_24LC64(char mem_H, char mem_L){
    I2C_STR();
    WriteI2C(ADR_24LC64_W);
    WriteI2C(mem_H);
    WriteI2C(mem_L);
}

void CloseStream_24LC64(){
    I2C_STP();
    I2C_PAUSE(10);
}

void WriteStream_24LC64(char data){
    WriteI2C(data);
    I2C_PAUSE(5000);
}

void WritePage_24LC64(char mem_H, char mem_L,char* data,unsigned int N){
    char i=0;
    if(N>=32)return;
    
    OpenWriteStream_24LC64(mem_H,mem_L);
    while(data[i] && !I2CError){
        WriteStream_24LC64(data[i++]);
    }
    CloseStream_24LC64();
}

int ReadFrom_24LC64(char mem_H, char mem_L){
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

void SequentialRead_24LC64(char mem_H, char mem_L,char *data, unsigned int N){
    char i=0;
    
    OpenReadStream_24LC64(mem_H,mem_L);
    while(i < N && !I2CError){
        if(i==N-1){
            data[i]==I2CRead(false);
            break;
        }
        data[i++]==I2CRead(true);
    }
    CloseStream_24LC64();
}

void OpenReadStream_24LC64(char mem_H, char mem_L){
    I2C_STR();
    WriteI2C(ADR_24LC64_W);
    WriteI2C(mem_H);
    WriteI2C(mem_L);
    I2C_STP();
    
    I2C_STR();
    WriteI2C(ADR_24LC64_R);
}

#endif	/* MC_26LC64_PIC */

