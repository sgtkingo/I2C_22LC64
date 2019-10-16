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

#define WBIT 0b11111110;
#define RBIT 0b11111111;

#define I2C_CLOCK  100000                    /* I2C clock frequency is 100 kHz  */

/* find bit rate to assign this value to SSPADD register, _XTAL_FREQ=FOSH */
#define BITRATE ((_XTAL_FREQ/(4*I2C_CLOCK))-1)

bit I2CError=false;
//Function DECLARATION:
void I2C_CLEARBUS();
void I2C_CLEARBUFFER();
void I2C_IDLE();
void MSSP_WAIT();
void I2C_INIT();
void I2C_STR();
void I2C_STP();
void WriteI2C( unsigned char data_out );
unsigned char I2CRead(char mode);


//Function DEFINITION:
void I2C_CLEARBUS(){   
    SSP1CON1bits.SSPEN = 0;   // Disable I2C operation
    I2C_CLEARBUFFER();
    
    TRISCbits.RC3=0; //SCL as OUT
    TRISCbits.RC4=0; //SDA as OUT
    SDA=SCL=0;
    TRISCbits.RC3=1; //SCL as IN
    TRISCbits.RC4=1; //SDA as IN
    
    //Check if bus still running
    if(!(SCL | SDA)){
        //stop if runnig
        I2C_STP();
    }
}

void I2C_CLEARBUFFER(){   
    SSP1BUF=0x00;
    PIR1bits.SSPIF=0;
}


void I2C_IDLE()
{
//check if I2C is in progress
while ( (SSP1STATbits.READ_WRITE | (SSP1CON2bits.SEN | SSP1CON2bits.PEN)) ) ; 
}

void MSSP_WAIT()
{   
    while(!PIR1bits.SSPIF);   // wait till SSPIF flag is set
    PIR1bits.SSPIF=0 ;
}

void I2C_INIT()
{
        ANSELCbits.ANSC3=0; //SCL as dig 
        ANSELCbits.ANSC4=0; //SDA as dig
        TRISCbits.RC3=1; //SCL as IN
        TRISCbits.RC4=1; //SDA as IN
        SDA=SCL=0;
        
        SSP1STAT = 0x00;      /* slew rate disabled, other bits are cleared*/
        SSP1CON1 = 0x28;     /* enable SSP port for I2C Master mode,
                                         * clock = FOSC / (4 * (SSPADD+1)) */
        SSP1ADD  = BITRATE;              /* clock 100 kHz */  
        
        INTCONbits.GIE=1;
        INTCONbits.PEIE=1;
        PIE1bits.SSPIE = 1;             /* enable SSPIF interrupt */
        PIR1bits.SSPIF = 0;             
}

void I2C_STR()
{ 
    SSP1CON1bits.SSPEN = 1;   // Enable I2C operation
    SSP1CON2bits.SEN=1; // Initialize the start condition
    I2C_IDLE();        // check if the I2C bus is idle
    MSSP_WAIT();       // wait till completion of event
}

void I2C_STP()
{
    SSP1CON2bits.PEN = 1;      // Initiate stop condition
    //MSSP_WAIT();            // Wait till completion of event
    I2C_CLEARBUFFER();
    I2C_IDLE();
    SSP1CON1bits.SSPEN = 0;   // Disable I2C operation
}


void WriteI2C( unsigned char data_out )
{
    SSP1BUF = data_out;           // write single byte to SSPBUF
    MSSP_WAIT();
    I2CError=SSP1CON2bits.ACKSTAT;
}


unsigned char I2CRead(char mode)
{
    char data=0;
    SSP1CON2bits.RCEN=1; //recieve
    SSP1CON2bits.ACKDT=!(mode & 0x01); //MODE TRUE=ACK, MODE FALSE=NACK
    
    MSSP_WAIT();
    data=SSP1BUF; //read data

    SSP1CON2bits.ACKEN=1;//ack sequence
    
    return data;        // Data from eeprom is now in the SSPBUF so return that value
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

