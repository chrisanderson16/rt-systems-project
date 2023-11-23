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

/**
 * Everything that needs to be generated for each time a picture is taken:
 *      [x] camera_name     // name of camera, done in main
        [x] file_format     // file extension
        [x] image_data      // description rather than actual image
        [x] currTime         
        [x] pic_name        // initialized to 0 in main
        [x] pic_height
        [x] pic_width
        [x] global_location; 
*/

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


/**
 * Hebron Oil Platform is at:
 *   47° 49' 23.8500'' N 53° 52' 21.1000'' W
 *              OR
 *   lat 47.82329167 long -53.87252778
 * 
 *  This is the equation for DMS -> DD (lat and longitude)
 *          DD = d + (min/60) + (sec/3600)
 * 
*/

    gps_t currPos;

// The follow coordinates are for the Hebron Oil Rig off the coast of Newfoundland
    currPos.latitude = 47.82329167;
    currPos.longitude = -53.87252778;

    camera.global_location = currPos;

    return camera;
}


