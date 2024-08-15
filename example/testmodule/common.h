#ifndef COMMON_H
#define COMMON_H

#include "math_tools.h"
#include "ray.h"
#include "vec3.h"

// Render settings
#define MAX_SAMPLES_PER_PIXEL 	5
#define MAX_RAY_DEPTH			5

// Image height calculated using aspect ratio
#define ASPECT_RATIO 			1.0/1.0
#define IMAGE_WIDTH 			754

// The height offset in the Pongo framebuffer
#define IMAGE_OFFSET			300

// Camera settings
#define DIST_TO_FOCUS 			10
#define APERTURE 				0.1

#endif