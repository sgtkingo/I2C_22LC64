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
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define SCL PORTCbits.RC3 //SCL init as 0
#define SDA PORTCbits.RC4 //SDA init as 0

#define F_OSC 16000000                        /* define F_OSC for finding bit rate*/
#define I2C_CLOCK  100000                    /* I2C clock frequency is 100 kHz  */
#define BITRATE ((F_OSC/(4*I2C_CLOCK))-1)    /* find bit rate to assign this value to SSPADD register */

#define _XTAL_FREQ 16000000 

void I2C_IDLE()
{
 //while ( (SSP1CON1 & 0b00011111) || (SSP1STAT & 0b00000100) ) ; 
while ( (SSP1STAT & 0b00000100) ) ; 
 //check the this on registers to make sure the                                                                 
 //IIC is not in progress
}

void MSSP_WAIT()
{   
    while(!PIR1bits.SSPIF);   // wait till SSPIF flag is set
    PIR1bits.SSPIF=0 ;
}

void I2C_Init()
{
        ANSELCbits.ANSC3=0; //SCL as dig 
        ANSELCbits.ANSC4=0; //SDA as dig
        TRISCbits.RC3=1; //SCL as IN
        TRISCbits.RC4=1; //SDA as IN
        SDA=SCL=0;
        SSP1STAT = 0b00000000;           /* slew rate disabled, other bits are cleared*/
        SSP1CON1  = 0x28;     /* enable SSP port for I2C Master mode,
                                         * clock = FOSC / (4 * (SSPADD+1)) */
        SSP1ADD  = BITRATE;              /* clock 100 kHz */  
        INTCONbits.GIE=1;
        INTCONbits.PEIE=1;
        PIE1bits.SSPIE = 1;             /* enable SSPIF interrupt */
        PIR1bits.SSPIF = 0;             
}

void I2C_STAT()
{ 
    //SSP1STATbits.S = 1; // Initialize the start condition 
    SSP1CON2bits.SEN=1;
    I2C_IDLE();        // check if the I2C bus is idle
    MSSP_WAIT();       // wait till completion of event
}

void WriteI2C( unsigned char data_out )
{
    SSP1BUF = data_out;           // write single byte to SSPBUF
    MSSP_WAIT();
}


unsigned char I2CRead()
{
    char data=0;
    SSP1CON2bits.RCEN=1; //recieve
    SSP1CON2bits.ACKDT=1; //no ACK
    
    MSSP_WAIT();
    data=SSP1BUF; //read data

    SSP1CON2bits.ACKEN=1;//ack sequence
    
    return data;        // Data from eeprom is now in the SSPBUF so return that value
}

void I2C_STP()
{
    SSP1CON2bits.PEN = 1;      // Initiate stop condition
    MSSP_WAIT();            // Wait till completion of event
    SSP1CON1bits.SSPEN = 0;   // Disable I2C operation
}

/*void main()
{
 I2C_Init();
 I2C_STAT();

 WriteI2C(0xD0);
    WriteI2C(0x88);
    WriteI2C(0xFF);
    I2C_STP();
}/*

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

