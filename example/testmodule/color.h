#ifndef COLOR_H
#define COLOR_H

#include <stdbool.h>

#include "vec3.h"
#include "ray.h"

typedef vec3 color;

bool hit_sphere(const point3 *center, double radius, const ray *r) {
    vec3 oc = vec3_subtraction(&r->orig, center);
    double a = vec3_dot(&r->dir, &r->dir);
    double b = 2.0 * vec3_dot(&oc, &r->dir);
    double c = vec3_dot(&oc, &oc) - radius * radius;
    double discriminant = b * b - 4 * a * c;
    return (discriminant >= 0);
}

color ray_color(const ray *r) {
    if (hit_sphere(get_vec3_ptr(make_vec3((double)0,(double)0,(double)-1)), 0.5, r)) {
        return make_vec3((double)1, (double)0, (double)0);
    }

    vec3 unit_direction = vec3_unit_vector(ray_direction(r));
    double a = 0.5 * (vec3_y(&unit_direction) + 1.0);
    
    color white = make_vec3((double)1.0, (double)1.0, (double)1.0);
    color blue = make_vec3(0.5, 0.7, (double)1.0);

    color weighted_white = vec3_scalar_multiply(1.0 - a, &white);
    color weighted_blue = vec3_scalar_multiply(a, &blue);

    return vec3_addition(&weighted_white, &weighted_blue);
}


#endif