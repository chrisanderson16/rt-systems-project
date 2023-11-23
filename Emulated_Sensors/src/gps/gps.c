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
    

/*
 * Satellite 1
 * 56, 30, 14.473 N
 * 35, 54, 38.735 W
 *
 * Satellite 2
 * 59,  9, 40.730 N
 * 88, 55, 45.985 W
 *
 * Satellite 3 
 * 27, 49, 30.396 N
 * 52, 27, 32.453 W
 */

//GPS signal is ~1.5 GHz, microwaves


int main(void)
{
// Initialize pseudo randomness
    srand(time(NULL));


    return 0;
}

