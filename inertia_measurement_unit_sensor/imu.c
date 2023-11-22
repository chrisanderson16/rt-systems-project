/*
* Title         : imu.c
* Author        : Chris Anderson
*                 B00795946
* Description   : This program is to emulate an Inertia Measurement Unit (IMU). Because IMU's use 6 degrees 
*                 of movement, at random times, there will be movements in any direction to show how the ROV
*                 has been moved or pushed around by the water.
*                 
*/


/*                                              Show the axis and rotations
* *************************************************************************************************************************

                             y-axis                                                   yaw                                    
                                |                                                      /(\                roll         
                               ((                                                     //((\           #                 
                             ,((((        z-axis                                     *******        ##%                 
                            /((((((    ###                                             /((       (###                   
                               ((    ###                                               /((     ###%                     
          #########            ((  ###                              #####               /((   ###(                       
             #########,        ((###                                  ########.       /(( ###                          
                #    \#######( (##                                            *#######//(%##         
                            .########      #                                          ########       
                           ### ((   ##########,                                     ####((   ########
                         ###   ((         #######                                 #### /((          ##(#####            
                       ###     ((                x-axis                         %##,   /((                  pitch
                     ###    (  ((  (                                          ###      /((         
                   ###       ((((((                                         #,#        /((         
                 ###          ((((                                       ####          /((                              
                               ((                                         #            /((                              


* *************************************************************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define PI atan(1)*4

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


int main(void)
{
    srand(time(NULL));

// We will define all as 0deg, this will be the ROV up-right facing north
    relative_t rov;

    // Initialize change flags
    rov.displacement_flag   = 0;
    rov.angular_flag        = 0;

    // Accelerometer (change relative position due to waves)
    rov.x_axis              = 0.0; // m
    rov.y_axis              = 0.0; // m
    rov.z_axis              = 0.0; // m

    // Gyroscope (when relative change of rotational position of ROV change)
    rov.x_axis_pitch        = 0.0; // dps (deg/s)
    rov.y_axis_yaw          = 0.0; // dps
    rov.z_axis_roll         = 0.0; // dps
    

    rov = change_pos(rov);
    rov = change_bearings(rov);

    if(rov.displacement_flag == 0)
    printf("x-axis = %f\ny-axis = %f\nz-axis = %f\n", \
            rov.x_axis, rov.y_axis, rov.z_axis);

    if(rov.angular_flag == 0)
    printf("pitch = %f\nyaw = %f\nroll = %f\n", \
            rov.x_axis_pitch, rov.y_axis_yaw, rov.z_axis_roll);


    return 0;
}


relative_t change_pos(relative_t currPos)
{

    int num_of_axis_to_change = rand() % 4;

    if(num_of_axis_to_change == 0)
    {
        currPos.displacement_flag = 1;
        printf("No positional change at this time.\n");
    }
    else
    {
        for(int i = 0; i < num_of_axis_to_change; i++)
        {
            int which_axis = rand() % 3;

            switch(which_axis)
            {
                case 0:
                // Do something here to change the x-axis
                    currPos.x_axis += ((float)(rand() % 10) / 20.0)*(powf((float)(-1.0),(float)(rand() % 2)));
                    break;
                case 1:
                // Do something here to change the y-axis
                    currPos.y_axis += ((float)(rand() % 10) / 20.0)*(powf((float)(-1.0),(float)(rand() % 2)));
                    break;
                case 2:
                // Do something here to change the z-axis
                    currPos.z_axis += ((float)(rand() % 10) / 20.0)*(powf((float)(-1.0),(float)(rand() % 2)));
                    break;
                default:
                // Should never get here
                    printf("Awkward, something went wrong with changing individual axis values\n");
                    break;
            }
        }
    }
    return currPos;
}



relative_t change_bearings(relative_t currBearings)
{
    int num_of_axis_bearings_to_change = rand() % 4;

    if(num_of_axis_bearings_to_change == 0)
    {   
        currBearings.angular_flag = 1;
        printf("No angular change at this time.\n");
    }
    else
    {
        for(int i = 0; i < num_of_axis_bearings_to_change; i++)
        {

            int which_axis_rotates = rand() % 3;

            switch(which_axis_rotates)
            {
                case 0:
                // Do something here to change the pitch
                    currBearings.x_axis_pitch += (((float)(rand() % 10) / 20.0)*(powf((float)(-1.0),(float)(rand() % 2)))) * (360/(2*PI));
                    break;
                case 1:
                // Do something here to change the yaw
                    currBearings.y_axis_yaw += (((float)(rand() % 10) / 20.0)*(powf((float)(-1.0),(float)(rand() % 2)))) * (360/(2*PI));
                    break;
                case 2:
                // Do something here to change the roll
                    currBearings.z_axis_roll += (((float)(rand() % 10) / 20.0)*(powf((float)(-1.0),(float)(rand() % 2)))) * (360/(2*PI));
                    break;
                default:
                // Should never get here
                    printf("Awkward, something went wrong with changing individual axis values\n");
                    break;
            }
        }
    }
    



    return currBearings;
}
