/*
 * Title         : main.c
 * Author        : Chris Anderson
 *                 B00795946 
 * Description   : This program is meant to demonstrate how each sensor may work in the final implementation.
 *                 That being said, the implementation of the system will still take time as each item will need
 *                 to be put into their specified task function. However, these tasks can simply call a function
 *                 that will run and initialize all we want.
 *                 
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "inc/pressure_sensor/pressure_sensor.h"



int main(void)
{
    srand(time(NULL));

    pressureSensorEmulator();


    return 0;
}


