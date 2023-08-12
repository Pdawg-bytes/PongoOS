#ifndef COLOR_H
#define COLOR_H

#include <stdbool.h>
#include <pongo.h>

#include "common.h"

typedef vec3 color;

double hit_sphere(const point3 *center, double radius, const ray *r) {
    vec3 oc = vec3_subtraction(&r->orig, center);
    double a = vec3_length_squared(&r->dir);
    double half_b = vec3_dot(&oc, &r->dir);
    double c = vec3_length_squared(&oc) - radius * radius;
    double discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (-half_b - square_root(discriminant) ) / a;
    }
}

color ray_color(const ray *r, const hittable* world) {
    vec3 center = make_vec3(0,0,-1);
    double t = hit_sphere(&center, 0.9, r);
    if (t > 0.0) {
        vec3 ray_point = ray_at(r, t);
        vec3 N = vec3_unit_vector(vec3_subtraction(&center, &ray_point));
        vec3 normalized_color = make_vec3(N.e[0] + 1, N.e[1] + 1, N.e[2] + 1);
        color pixel_color = vec3_scalar_multiply(0.5, &normalized_color);
        return pixel_color;
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