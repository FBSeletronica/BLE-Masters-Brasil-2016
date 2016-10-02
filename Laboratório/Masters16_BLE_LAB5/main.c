/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB® Code Configurator

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB® Code Configurator - v2.25.2
        Device            :  PIC16F1619
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 v1.34
        MPLAB             :  MPLAB X v2.35 or v3.00
 */

/*
Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 */

#include "mcc_generated_files/mcc.h"
#define cmdLONGDELAY    640  // 640ms

char rxBuffer[32];

void mygets( void);
void sendCMD( const char * cmd);

/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    // initialize RN4020
    CMD_SetHigh();           //command mode
    WAKE_SW_SetHigh();       // enter command mode
    mygets();                //wait response  
    sendCMD( "SF,2");        // factory reset
    sendCMD( "SS,10000000"); // service Herat Rate (20000000)
    sendCMD( "SR,20000000"); // set features
    sendCMD( "S-,Masters16"); // change name
    sendCMD( "R,1");         // reboot with new settings
    mygets();
    __delay_ms( cmdLONGDELAY);   // wait for CMD prompt
    while( !CONN_GetValue());   // wait for a connection
    
    while (1)
    {
        uint8_t time;              // auxiliary time variable
                    
        if ( TMR2_HasOverflowOccured()) { 
            time++;                         
        }
    
        if ( time >= 50)   // 200ms
        { 
            uint8_t value; 
            time = 0;
            LED_D6_Toggle();   
            
            value =  ADC1_GetConversion(POT)>>9; // 0 to 127
           
           //Health Thermometer 
           printf( "SUW,2A1C,01%02x000000\n",value);
           mygets();
           
           //temperature type
           printf( "SUW,2A1D,%02x\n",2);
           mygets();       

        }
    }
}

void mygets( void)
{
    char c, i=0;
    char *p = rxBuffer;
    while ( (i++ < sizeof(rxBuffer)) && (( c = getch()) != '\n')) {
        *p++ = c;
    }
    *p = '\0';
}

void sendCMD( const char * cmd)
{
    puts( cmd); 
    mygets(); 
}
/**
 End of File
 */