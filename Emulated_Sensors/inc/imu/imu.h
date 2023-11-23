/*
 *  Title: imu.h 
 *  Author: Chris Anderson
 *  Description: Header file for imu.c
 *  
*/


#ifndef __IMU_H
#define __IMU_H

typedef struct relative_pos_bearings
{
    // Flags to detect if change has occurred
    int displacement_flag;
    int angular_flag;
    
    // Accelerometer params (relative position)
    float x_axis;       // m
    float y_axis;       // m
    float z_axis;       // m

    // Gyroscope parms (relative bearings)
    float x_axis_pitch; // dps (deg/s)
    float y_axis_yaw;   // dps
    float z_axis_roll;  // dps
    
}relative_t;


relative_t change_pos(relative_t currPos);
relative_t change_bearings(relative_t currBearings);



#endif
