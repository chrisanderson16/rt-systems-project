/*
 *  Title: camera.h 
 *  Author: Chris Anderson
 *  Description: Header file for camera.c
 *  
*/


#ifndef CAMERA_H
#define CAMERA_H

#include "../gps/gps.h"

typedef struct camera_metadata
{
    char * camera_name; // This will be initialized in main
    char * file_format; // File format of the image.
    char * image_data;     // This will just be a dummy variable that "holds" the image data
    char * currTime;    // Time will be attached to the photo

    int pic_name;       // We give the picture a number for a name
    int pic_height;     // The size of the image, in pixels
    int pic_width;


    gps_t global_location;   // These values will be "GPS" location of the photo

}camera_t;


camera_t take_photo(camera_t camera);



#endif