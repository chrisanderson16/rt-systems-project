/*
 *  Title: gps.h 
 *  Author: Chris Anderson
 *  Description: Header file for gps.c
 *  
*/


#ifndef GPS_H
#define GPS_H

#include <time.h>

// Class that will be used to keep track of location
typedef struct gps_notation
{


// ######## CHANGE THIS TO LAT AND LONG

// This will be used further with the GPS sensor module, for now it is just a way to display the information
    int degree;
    int minute;
    float second;
    char cardinal_direction;

}dms_t; // degree, minute, seconds and assume N and W respectively.


// This will be run 3 times to get 3 different satellite
dms_t * init_satellite(dms_t * all_satellites);

// This will be run 3 times to get the time to each satellite
time_t time_to_satellite(dms_t satellite);

// This will triangulate the position based on each satellite
dms_t get_receiver_dms(dms_t * satellites);

#endif