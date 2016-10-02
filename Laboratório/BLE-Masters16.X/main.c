/**
 * Project  : RN4020 - BLE2 Click Board Test
 * File Name:   main.c
 * 
 * Summary: This is the main file to interface the RN4020 module 
 *          on the Curiosity board
 * 
 * LED D5 : LED, comunication status
 * 
 * BLE_  : This is the RN4020 unit with the following pins:
 *            CONN: RC2
 *            WKSW: RA4
 *            CMD/MLDP: RC5
 *            TX:   RB7
 *            RX:   RB5
 * 
 */

#include "mcc_generated_files/mcc.h"

#define cmdLONGDELAY    640  // 640ms

char rxBuffer[32];

void SendBatteryValue(uint8_t value);
void SendHeartHateValue(uint8_t bpm, uint8_t energy,uint8_t RR,uint8_t BodySensorLocation);
void SendHealthThermometer(uint8_t temperature, uint8_t type);
void mygets( void);
void sendCMD( const char * cmd);


void main(void) 
{
    SYSTEM_Initialize();        //init system
        
    // initialize RN4020
    WAKE_SW_SetHigh();       // enter command mode
    mygets();                //wait response  
    sendCMD( "SF,2");        // factory reset
    sendCMD( "SS,F0000000"); // service
    sendCMD( "SR,20000000"); // set features
    sendCMD( "S-,Masters16"); // change name
    sendCMD( "R,1");         // reboot with new settings
    mygets();
    __delay_ms( cmdLONGDELAY);   // wait for CMD prompt
    while( !CONN_GetValue());   // wait for a connection
      
    // main loop
    while (1) {
        
        if(!CONN_GetValue()){          //if not connected
           
           LED_D5_SetLow();            // clear LED
           while( !CONN_GetValue());   // wait for a connection
        }
        else{                          //if connected
       
            uint8_t time;              // auxiliary time variable
            
        if ( TMR2_HasOverflowOccured()) { // time is measured in multiples of TMR2 period (4ms)
            time++;                         
        }
    
        if ( time >= 50)   // 200ms
        {  
            uint8_t value =  ADC1_GetConversion( POT)>>9;
            time = 0;
            
            //batery        
            SendBatteryValue(value);      
            
            //Heart hate
            SendHeartHateValue(value,180,235,1);
            
            //Health Thermometer
            SendHealthThermometer(value, 2);
            
            LED_D5_Toggle();
         }
            
        }
        
        
    }
}

void SendBatteryValue(uint8_t value){
    
    printf( "SUW,2A19,%02x\r\n", (value>100)? 100: value); // update the battery level 
    mygets();                                              //wait response              
    
}

void SendHeartHateValue(uint8_t bpm, uint8_t energy,uint8_t RR, uint8_t BodySensorLocation){
    
    printf( "SUW,2A37,1F%02x00%02x00%02x00\n", bpm,energy,RR);
    mygets();
    
    //Sensor location
    printf( "SUW,2A38,%02x\n",BodySensorLocation);
    mygets(); 
    
}

void SendHealthThermometer(uint8_t temperature, uint8_t type){
    
    printf( "SUW,2A1C,01%02x000000\n",temperature);
    mygets();
            
    printf( "SUW,2A1D,%02x\n",type );
    mygets();
    
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