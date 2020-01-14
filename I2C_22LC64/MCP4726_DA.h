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
#ifndef MCP4726_DA_PIC
#define MCP4726_DA_PIC

#include "I2C.h"
#define default_adr_MCP4726 0b11000010

#define CMD_DAC 0b000
#define CMD_COMMAND 0b010
#define CMD_ALL 0b011
#define CMD_CONF 0b100

#define PD_NORMAL 0b00
#define PD_1k 0b01
#define PD_125k 0b10
#define PD_640k 0b11

#define VR_VDD 0b00
#define VR_VREF_UBF 0b10
#define VR_VREF_BF 0b11

#define Gx1 0
#define Gx2 1

#define EEPROM_MEM_SIZE_4726 6


char ADR_4726=0;
char DEFAULT_COMMAND=0;

char EEPROM_MEM_4726[EEPROM_MEM_SIZE_4726];

// address_of_24LC64_in the BUS I2C, W	
#define	ADR_4726_W (ADR_4726 MASK_W)	
// address_of_24LC64_in the BUS I2C, R	
#define	ADR_4726_R (ADR_4726 MASK_R)

//Intit EEPROM, enter address or set to default
void Init_MCP4726(char address);
void InitClear_MCP4726();

void WriteTo_MCP4726(char data);
int ReadFrom_MCP47264();

void OpenStream_MCP4726();
void CloseStream_MCP4726();
void WriteStream_MCP4726(char data);

void Write_DAC_MCP4726(char pd, unsigned int data);
void Write_ALL_MCP4726(char cmd, unsigned int data);
void Write_CMD_MCP4726(char cmd);

char CreateCommand_MCP4726(char CMD, char VR, char PD, char G);
void Set_VoltageAsData_MCP4726(unsigned int voltage_data);



void Init_MCP4726(char address){
    ADR_4726=address;
    I2C_INIT();
    I2C_CLEARBUS();
    
    InitClear_MCP4726();
}

char CreateCommand_MCP4726(char CMD, char VR, char PD, char G){
    char command=0x00;
    
    command|=CMD<<5;
    command|=VR<<3;
    command|=PD<<1;
    command|=G;
    
    return command;
}

void Set_VoltageAsData_MCP4726(unsigned int voltage_data){
    Write_DAC_MCP4726(PD_NORMAL,voltage_data);
}

void InitClear_MCP4726(){
    DEFAULT_COMMAND=CreateCommand_MCP4726(CMD_ALL,VR_VDD,PD_NORMAL,Gx1);
    Write_ALL_MCP4726(DEFAULT_COMMAND,0);
}

void WriteTo_MCP4726(char data){
    I2C_STR();
    WriteI2C(ADR_4726_W);
    WriteI2C(data);
    I2C_STP();
}

void OpenStream_MCP4726(){
    I2C_STR();
    WriteI2C(ADR_4726_W);
}

void CloseStream_MCP4726(){
    I2C_STP();
}

void WriteStream_MCP4726(char data){
    WriteI2C(data);
}

void Write_DAC_MCP4726(char pd, unsigned int data){
    char firstB=0x00;char secondB=0x00;
    firstB=pd<<4;
    firstB|=(data & 0x0F00);
    secondB=(data & 0x00FF);
    
    OpenStream_MCP4726();
    WriteStream_MCP4726(firstB);
    WriteStream_MCP4726(secondB);
    CloseStream_MCP4726();
}

void Write_ALL_MCP4726(char cmd, unsigned int data){
    OpenStream_MCP4726();
    WriteStream_MCP4726(cmd);
    WriteStream_MCP4726(char(data>>8));
    WriteStream_MCP4726(char(data & 0x000F));
    CloseStream_MCP4726();
}

void Write_CMD_MCP4726(char cmd){
    WriteTo_MCP4726(cmd);
}

void ReadFrom_MCP4726(){
    
    I2C_STR();
    WriteI2C(ADR_4726_R);
    
    for(int i=0;i<EEPROM_MEM_SIZE_4726;i++){
        EEPROM_MEM_4726[i]=I2CRead(true);
    }
    I2C_STP();
}

#endif	/* MCP4726_DA_PIC */

