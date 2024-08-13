#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <math.h>
#include <limits.h>
#include <float.h>

// Constants

const double infinity_def = INFINITY;  // Use a different name to avoid clash
const double pi = 3.1415926535897932385;

// Utility Functions

static inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

#include "ray.h"
#include "vec3.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#endif