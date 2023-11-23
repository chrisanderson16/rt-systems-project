/*
* Title         : camera.c
* Author        : Chris Anderson
*                 B00795946
* Description   : This program is designed to emulate a camera on an underwater wielding robot (rov). It will be able
*                 to "take a picture" of what it's looking at, and "return" the image and relevant metadata.
*                 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "../../inc/camera/camera.h"


/*
int main(void)
{

// Initialize camera to take pictures
    camera_t cam;

// Set camera name
    char * camera_model = "Emulated Camera";
    cam.camera_name = camera_model;

// Set first picture name to 0
    int img_name = 0;
    cam.pic_name = img_name;


    cam = take_photo(cam);

    //printf("Camera name is %s\n", cam.camera_name);

    //printf("Curr System time is %s\n", cam.currTime);


    return 0;
}
*/

camera_t take_photo(camera_t camera)
{
// In this function, we want to generate pseudo random image metadata

    // Change picture name
    int img_name = camera.pic_name + 1;
    camera.pic_name = img_name;


    // Set picture format to jpg
    char * f_format = "jpeg";
    camera.file_format = f_format;


    // Set picture size
    camera.pic_height = 600;
    camera.pic_width = 800;


    // Set time of picture taken
    time_t mytime = time(NULL);
    char * time_str = ctime(&mytime);
    time_str[strlen(time_str)-1] = '\0';
    camera.currTime = time_str;


    // Photo description in place of actual image data
    char * image_taken = "A nice hunk of metal that needs to be wielded.";
    camera.image_data = image_taken;


    // Initialize new GPS location
    dms_t currPos[2];

// ############# CHANGE TO LAT AND LONG


// The follow coordinates are for the Hebron Oil Rig off the coast of Newfoundland
    currPos[0].degree = 47;
    currPos[0].minute = 49;
    currPos[0].second = 23.85;
    currPos[0].cardinal_direction = 'N';

    currPos[1].degree = 53;
    currPos[1].minute = 52;
    currPos[1].second = 21.1;
    currPos[1].cardinal_direction = 'W';

    // Set camera's/rov's location to these coords
    camera.global_location[0] = currPos[0];
    camera.global_location[1] = currPos[1];

    return camera;
}


