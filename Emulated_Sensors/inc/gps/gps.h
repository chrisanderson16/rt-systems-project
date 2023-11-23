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
    // Maybe use this?
    int ID;

    // This will be used further with the GPS sensor module, for now it is just a way to display the information
    float latitude;
    float longitude;
    float altitude;

    time_t timeAtTx;

}gps_t; // Decimal degrees are easier than multiple DMS



gps_t delta_receiver_satellite(gps_t receiver, gps_t satellite);
float find_time_to_receiver(gps_t abs_dist);



#endif