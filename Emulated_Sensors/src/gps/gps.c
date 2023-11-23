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

// This will be run 3 times to get 3 different satellite
dms_t * init_satellite(dms_t * satellite)
{
    
    satellite[0].degree = 7;
    
    
    return NULL;
}

// This will be run 3 times to get the time to each satellite
time_t time_to_satellite(dms_t satellite)
{

}

// This will triangulate the position based on each satellite
dms_t get_receiver_dms(dms_t * satellites)
{

}


int main(void)
{
// Initialize pseudo randomness
    srand(time(NULL));

// Receiver of location we need to find
    dms_t receiver;

// Satellites are in an array
    dms_t * satellite1[2];
    dms_t * satellite2[2];
    dms_t * satellite3[2];


    satellite1 = init_satellite(satellite1);



    return 0;
}

