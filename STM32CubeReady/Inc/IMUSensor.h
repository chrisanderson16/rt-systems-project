/*
 * PressureSensor.h
 *
 *  Created on: Nov 23, 2023
 *      Author: chris
 */

#ifndef INC_USER_L3_PRESSURESENSOR_H_
#define INC_USER_L3_PRESSURESENSOR_H_

#include "FreeRTOS.h"
#include "timers.h"

typedef struct six_degrees_of_freedom
{
    // Accelerometer params (relative position)
    float x_axis;       // m
    float y_axis;       // m
    float z_axis;       // m

    // Gyroscope parms (relative bearings)
    float x_axis_pitch; // dps (deg/s)
    float y_axis_yaw;   // dps
    float z_axis_roll;  // dps

}relative_t;

void RunIMUSensor(TimerHandle_t xTimer);

#endif /* INC_USER_L3_PRESSURESENSOR_H_ */
