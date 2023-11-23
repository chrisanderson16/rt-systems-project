/*
* Title         : gps.c
* Author        : Chris Anderson
*                 B00795946
* Description   : This program is going to emulate the initialization of GPS signal, and output a gps location. 
*                 This will be done by having 3 static satellites, and then triangulating the position of the gps
*                 receiver.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../inc/gps/gps.h"

#include <math.h>


#define c_speed 300000000 // Speed of light (signal traveling from satellite to receiver)

// #########CHANGE TO LAT AND LONG

/**
 * Satellites send the following data:
 *      Latitude
 *      Longitude
 *      Height
 *      
 *      Current Time on Satellite
 * 
 * Receiver needs to:
 *      At time message is received, subtract current time from that to find distance between
 *      
 *      Do that for at least 4, then use their Lat and Long to find where receiver is.
 * 
 * */ 
    

/**
 * Assume all satellite heights are ~20.2km above the device
 * 
 * Latitude      Longitude
 * #######################
 * 
 * Satellite 1
 * 61.371464, -82.219258
 *
 * Satellite 2
 * 29.320890, -81.890649
 *
 * Satellite 3 
 * 32.724449, -20.256573
 * 
 * Satellite 4
 * 66.555259, -18.212847
 * 
 * Assume receiver is at sea level
 * 
 * Receiver actual location
 * 47.823292, -53.872528
 * 
 * We first need to find the time to emulate what is actually happening, for this, we assume the earth is flat, so that we are 
 * moreso working in XYZ coords, rather than on a spherical surface
 */

//GPS signal is ~1.5 GHz, microwaves

gps_t delta_receiver_satellite(gps_t receiver, gps_t satellite);
float find_time_to_receiver(gps_t abs_dist);
/*
int main(void)
{
// Initialize pseudo randomness
    srand(time(NULL));

    gps_t receiver;
    receiver.latitude       = 47.823292;    // z-axis (2D-vertical)     (DD)
    receiver.longitude      = -53.872528;   // x-axis (2D-horizontal)   (DD)
    receiver.altitude       = 0.0;          // y-axis (3D-towards)      (meters)

    gps_t satellite_1;
    satellite_1.latitude    = 61.371464;
    satellite_1.longitude   = -82.219258;
    satellite_1.altitude    = 20200.0;

    gps_t satellite_2;
    satellite_2.latitude    = 29.320890;
    satellite_2.longitude   = -81.890649;
    satellite_2.altitude    = 20200.0;

    gps_t satellite_3;
    satellite_3.latitude    = 32.724449;
    satellite_3.longitude   = -20.256573;
    satellite_3.altitude    = 20200.0;

    gps_t satellite_4;
    satellite_4.latitude    = 66.555259;
    satellite_4.longitude   = -18.212847;
    satellite_4.altitude    = 20200.0;


//---------------------------------- TIME TO SATELLITE 1 ----------------------------------------------------------
    gps_t distance_to_receiver = delta_receiver_satellite(receiver, satellite_1);

//    printf("Different between receiver and satellite1: \n\
//   deltaZ = %f\n   deltaX = %f\n   deltaY = %f", \
//    distance_to_receiver.latitude, distance_to_receiver.longitude, distance_to_receiver.altitude);

    float time_to_receiver = find_time_to_receiver(distance_to_receiver);

    printf("\nIt will take %fs to get from satellite_1 to receiver\n", time_to_receiver);

//---------------------------------- TIME TO SATELLITE 2 ----------------------------------------------------------
    distance_to_receiver = delta_receiver_satellite(receiver, satellite_2);
    time_to_receiver = find_time_to_receiver(distance_to_receiver);

    printf("\nIt will take %fs to get from satellite_2 to receiver\n", time_to_receiver);

//---------------------------------- TIME TO SATELLITE 3 ----------------------------------------------------------
    distance_to_receiver = delta_receiver_satellite(receiver, satellite_3);
    time_to_receiver = find_time_to_receiver(distance_to_receiver);

    printf("\nIt will take %fs to get from satellite_3 to receiver\n", time_to_receiver);

//---------------------------------- TIME TO SATELLITE 4 ----------------------------------------------------------
    distance_to_receiver = delta_receiver_satellite(receiver, satellite_4);
    time_to_receiver = find_time_to_receiver(distance_to_receiver);

    printf("\nIt will take %fs to get from satellite_4 to receiver\n", time_to_receiver);

    return 0;
}
*/
/**
 * 0.001 DD = 111m --> 111000m/DD
*/


//####################################Just to find the time to set static for each satellite#################################

gps_t delta_receiver_satellite(gps_t receiver, gps_t satellite)
{
// Initialize difference coord to find absolute distance between receiver and x-satellite
    gps_t abs_diff;

    // Set to zero to avoid any hiccups
    abs_diff.latitude   = 0;
    abs_diff.longitude  = 0;
    abs_diff.altitude   = 0;

    // Find the absolute difference between distances, the convert to meters
    abs_diff.latitude   = abs(satellite.latitude - receiver.latitude) * 111000.0;
    abs_diff.longitude  = abs(satellite.longitude - receiver.longitude) * 111000.0;
    abs_diff.altitude   = abs(satellite.altitude - receiver.altitude);

    return abs_diff;
}

float find_time_to_receiver(gps_t abs_dist)
{
    float hypotenuse = sqrt(pow(abs_dist.latitude, 2) + pow(abs_dist.longitude, 2));
    float final_dist = sqrt(pow(abs_dist.altitude, 2) + pow(hypotenuse, 2));
    printf("\nSatellite Dist to Receiver = %f", final_dist);

    float time_in_sec_from_satellite_receiver = final_dist / c_speed;

    return time_in_sec_from_satellite_receiver;

}