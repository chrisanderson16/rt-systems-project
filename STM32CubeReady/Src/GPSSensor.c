/*
 * GPSSensor.c
 *
 *  Created on: Nov 23, 2023
 *      Author: Chris Anderson
 */

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "User/L2/Comm_Datalink.h"
#include "User/L3/GPSSensor.h";

//Required FreeRTOS header files
#include "FreeRTOS.h"
#include "Timers.h"


/******************************************************************************
This is a software callback function.
******************************************************************************/
void RunGPSSensor(TimerHandle_t xTimer)
{
	srand(time(NULL));

	static gps_t satellite[3];

    satellite[0].latitude    = 61.371464;
    satellite[0].longitude   = -82.219258;
    satellite[0].altitude    = 20200.0;
    satellite[0].timeToRx    = 0.011422;

    satellite[1].latitude    = 29.320890;
    satellite[1].longitude   = -81.890649;
    satellite[1].altitude    = 20200.0;
    satellite[1].timeToRx    = 0.012316;

    satellite[2].latitude    = 32.724449;
    satellite[2].longitude   = -20.256573;
    satellite[2].altitude    = 20200.0;
    satellite[2].timeToRx    = 0.013412;

    satellite[3].latitude    = 66.555259;
    satellite[3].longitude   = -18.212847;
    satellite[3].altitude    = 20200.0;
    satellite[3].timeToRx    = 0.014562;

    uint16_t pick_rand_satellite = rand() % 4;

    switch(pick_rand_satellite)
    {
    	case 0:
    		// Transmit these values
    		send_sensorData_message(GPS, satellite[0]);
    		break;
    	case 1:
    		// Transmit these values
    		send_sensorData_message(GPS, satellite[1]);
    		break;
    	case 2:
    		// Transmit these values
    		send_sensorData_message(GPS, satellite[2]);
    		break;
    	case 3:
    		// Transmit these values
    		send_sensorData_message(GPS, satellite[3]);
    		break;
    	default:
    		//Should never get here
    		break;
    }

    // We can add a task delay here
    vTaskDelay(5);
}


