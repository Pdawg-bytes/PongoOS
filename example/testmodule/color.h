#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "ray.h"

typedef vec3 color;

color ray_color(const ray *r) {
    vec3 unit_direction = vec3_unit_vector(ray_direction(r));
    double a = 0.5 * (vec3_y(&unit_direction) + 1.0);
    
    color white = make_vec3((double)1.0, (double)1.0, (double)1.0);
    color blue = make_vec3(0.5, 0.7, (double)1.0);

    color weighted_white = vec3_scalar_multiply(1.0 - a, &white);
    color weighted_blue = vec3_scalar_multiply(a, &blue);

    return vec3_addition(&weighted_white, &weighted_blue);
}


#endif