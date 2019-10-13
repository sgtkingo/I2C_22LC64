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
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef I2C_HEADER_TEMPLATE_H
#define	I2C_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define SCL PORTCbits.RC3 //SCL init as 0
#define SDA PORTCbits.RC4 //SDA init as 0

bit i2cError; //1 ERROR, 0 OK
    
void waitI2C(void);
void initI2C(void);
void setupI2C(void);

int startI2C(void);
void stopI2C(void);

void writeI2C(char data);
char readI2C(void);


void waitI2C(void){
    while(!SSPIF);
    SSPIF=0;
}

void initI2C(void){
    //INTCON|=0b11000000; //int enable bits
    //PIE1bits.SSP1IE=1; //SSP int enable
    
    ANSELCbits.ANSC3=0; //SCL as dig 
    ANSELCbits.ANSC4=0; //SDA as dig
    TRISCbits.RC3=1; //SCL as IN
    TRISCbits.RC4=1; //SDA as IN
    SDA=SCL=0;
    
    SSP1CON1=0b00100000; //Set SSPI as I2C, Master mode FOSH/4
    SSP1CON2=0x00; //Set inter processing of I2C (ACK, Stop/Start), IMPORTANT REGISTR USABLE IN PROGRAM!!!
    SSP1CON3=0b01101000; //INT setting for I2C, IMPORTANT REGISTR USABLE IN PROGRAM!!!
    SSP1ADD=0b00100111; //SSP freq set to: 100 kHz
    SSP1STAT=0x00;
    
    setupI2C();
    //SSP1BUF buffer for write/read to/from I2C
    //SSP1STAT
}
void setupI2C(void){
    SSP1CON1bits.SSPEN=0;
    if(SDA){
       TRISCbits.RC3=0;
       TRISCbits.RC4=0;
       SSP1CON1bits.SSPEN=1;
    }
    
}

int startI2C(void){
    SSP1CON2bits.SEN=1; //start event
    
    i2cError=0;
    for(int i=0;i<10;i++){
        if((SDA&&SCL))SSP1CON2bits.RSEN=1; //re-start event
        else break;
    }
    if((SDA&&SCL))i2cError=1;
    return !i2cError;
}

void stopI2C(void){
    SSP1CON2bits.PEN=1; //stop event
    
    i2cError=0;
    for(int i=0;i<10;i++){
        if(!(SDA&&SCL))SSP1CON2bits.PEN=1;
        else break;
    }
    if(!(SDA&&SCL))i2cError=1;
}

void writeI2C(char data){
    SSP1BUF=data; //send data
    waitI2C();
    i2cError=(SSP1CON2bits.ACKSTAT);//ACKSTAT=0 if ACK OK
}

char readI2C(void){
    char data=0;
    SSP1CON2bits.RCEN=1; //recieve
    SSP1CON2bits.ACKDT=1; //no ACK
    
    waitI2C();
    data=SSP1BUF; //read data
    waitI2C();
    SSP1CON2bits.ACKEN=1;//ack sequence
    
    return data;
}


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* I2C_HEADER_TEMPLATE_H */

