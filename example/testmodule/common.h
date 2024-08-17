#ifndef COMMON_H
#define COMMON_H

#include "math_tools.h"
#include "ray.h"
#include "vec3.h"

// Render settings
#define MAX_SAMPLES_PER_PIXEL 	10
#define MAX_RAY_DEPTH			10

// Camera settings
#define DIST_TO_FOCUS 			10
#define APERTURE 				0

// The height offset in the Pongo framebuffer
#define IMAGE_OFFSET			300

// Image height calculated using aspect ratio
#define ASPECT_RATIO 			(1.0/1.0)
#define IMAGE_WIDTH 			750
// Image width should match the physical width of your iDevice's screen.
// Table of resolutions (arm64 checkm8 devices):
/*
 * iPhone -------
 * 640 x 1136 ---
 * 5s, SE 1
 * 
 * 750 x 1334 ---
 * 6, 6s, 7, 8
 * 
 * 1080 x 1920
 * 6 Plus, 6s Plus, 7 Plus, 8 Plus
 * 
 * 1125 x 2436
 * X
 * 
 */
/*
 * iPod Touch ---
 * 640 x 1136 ---
 * 6,7
 * 
 */
/* 
 * iPad ---------
 * 1536 x 2048 --
 * Air 1, Air 2;
 * mini 1, mini 2, mini 3;
 * 5, 6, 7;
 * Pro 12.9" 1, Pro 9.7" 1, Pro 12.9" 2, Pro 10.5"
*/

#endif