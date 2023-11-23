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
#include "inc/imu/imu.h"
#include "inc/camera/camera.h"



int main(void)
{
    srand(time(NULL));

//############## PRESSURE SENSOR #######################################

    //pressureSensorEmulator();


// ################## IMU #################################################

/*
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

*/
//#################################CAMERA######################################

// Initialize camera to take pictures
    camera_t cam;

// Set camera name
    char * camera_model = "Emulated Camera";
    cam.camera_name = camera_model;

// Set first picture name to 0
    int img_name = 0;
    cam.pic_name = img_name;


    cam = take_photo(cam);


    /**
 * Everything that needs printed:
 *      [x] camera_name     // name of camera, done in main
        [x] file_format     // file extension
        [x] image_data      // description rather than actual image
        [x] currTime         
        [x] pic_name        // initialized to 0 in main
        [x] pic_height
        [x] pic_width
        [x] global_location; 
*/

    printf("Picture taken with %s at time %s\n",cam.camera_name,cam.currTime);
    printf("Show Photo: \n    %s\n", cam.image_data);
    printf("Show Photo Metadata: \n    Captured with: %s\n    Time: %s\n    Name: %d.%s\n    Dimensions: %d x %d\n    Location: Lat %f and Long %f\n", \
            cam.camera_name, cam.currTime, cam.pic_name, cam.file_format, cam.pic_width, cam.pic_height, \
            cam.global_location.latitude, cam.global_location.longitude);
    


    return 0;
}


