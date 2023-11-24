/*
 * PressureSensor.c
 *
 *  Created on: Nov 23, 2023
 *      Author: Chris Anderson
 */

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "User/L2/Comm_Datalink.h"
#include "User/L3/PressureSensor.h";

//Required FreeRTOS header files
#include "FreeRTOS.h"
#include "Timers.h"



/******************************************************************************
This is a software callback function.
******************************************************************************/
void RunPressureSensor(TimerHandle_t xTimer)
{
	srand(time(NULL));

	static uint32_t currentPressure = 0.0;
	static uint32_t currentVoltage = 0.0;

	if(currentPressure == 0.0){
		for(int i = 0; i < 180; i++)
		{
			currentPressure += (float)i;
			currentVoltage = currentPressure * 0.1; // This is the conversion for emulated pressure sensor (~0.1mV per 1 atm)
			vTaskDelay(0.3);
		}
	}

	uint16_t up_or_down = rand() % 2;

	if(up_or_down < 1)
	{
// If number is 0, the pressure goes down
		currentPressure--;
	}else{
// If number is 1, the pressure goes up
		currentPressure++;
	}

	currentVoltage = currentPressure * 0.1; // This is the conversion for emulated pressure sensor (~0.1mV per 1 atm)

	send_sensorData_message(Pressure, currentVoltage); // CHANGE DEPTH TO PRESSURE
}
