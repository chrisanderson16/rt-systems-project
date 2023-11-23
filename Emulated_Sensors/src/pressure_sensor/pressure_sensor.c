/*
* Title         : pressure_sensor.c
* Author        : Chris Anderson
*                 B00795946
* Description   : This program is designed to emulate a pressure sensor for an underwater wielding robot. 
*                 This will be done by emulating depths, randomizing whether it will go up or down by x 
*                 meters, then giving that to a function that will return a voltage based on the current depth.
*
*                 This function returns an output voltage in mV.
*/

// Note: these libraries and functions may change when moving to FreeRTOS
 

// A potential random function in FreeRTOS would be: 

// #include <FreeRTOS_sockets.h>
// BaseType_t xApplicationGetRandomNumber( uint32_t * pulNumber);
//              + pulNumber is filling with random number, if function passes

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "/home/chris/school/rt-systems/project/inc/pressure_sensor.h"

// This is where our program will change pressure based on "movement"
void pressureSensorEmulator(void)
{
    // Assume vessel starts at the relative surface
    float initPressure      = 0.0;          // atm (gauge)
    float currPressure      = 0.0;          // atm (gauge)
    float initVoltage       = 100;          // mV
    float currVoltageOut    = initVoltage;  // mV

    const float mV_per_atm  = 0.1;          // mV / atm

// NOTE: It may help to insert a xTaskDelay for each for loop as to make the descent slower, maybe?

// Assume avg sea floor depth is 3700m, go to middle for wielding work
// We also go down 1 atm of pressure at a time, or 10m, so 180 atm is middle way
    for(int i = initPressure; i <= 180.0; i++)
    {
        currPressure = (float)i; 
        currVoltageOut = currPressure * mV_per_atm;
        //printf("i = %d\nCurrent Pressure = %fatm\nCurrent output Voltage = %f\n\n", i, currPressure, currVoltageOut);
    }

// Now, we're at the level we want to be at, the for-loop can probably be removed, it was more for debugging
    
    for(int i = 0; i < 200; i++)
    {

// We pick a random number from 0-100, and if the number is higher, the rov moved up (pressure decrease), lower, the rov moved down(pressure increased)    
        int r = rand() % 100;

        if(r < 50)
        {
        
            currPressure -= 1; // We moved up, so the pressure is decreased
            currVoltageOut = currPressure * mV_per_atm;
      
            
        }else{

            currPressure += 1; // We moved down, so the pressure is increased
            currVoltageOut = currPressure * mV_per_atm;
      

        }


        printf("low rand\noutput voltage = %f\n\n", currVoltageOut);

    }

    return;
}

int main(void)
{
// Initialize random number
    srand(time(NULL));

// Run pressure program
    pressureSensorEmulator();

    return 0;
}