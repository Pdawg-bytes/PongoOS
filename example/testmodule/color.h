#ifndef COLOR_H
#define COLOR_H

#include <stdbool.h>
#include <pongo.h>

#include "vec3.h"
#include "ray.h"

typedef vec3 color;

double hit_sphere(const point3 *center, double radius, const ray *r) {
    vec3 oc = vec3_subtraction(&r->orig, center);
    double a = vec3_dot(&r->dir, &r->dir);
    double b = 2.0 * vec3_dot(&oc, &r->dir);
    double c = vec3_dot(&oc, &oc) - radius * radius;
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        printf("Did we make it here?");
        return -1.0;
    }
    else {
        return (-b - square_root(discriminant)) / (2.0 * a);
    }
}

color ray_color(const ray *r) {
    double t = hit_sphere(get_vec3_ptr(make_vec3(0, 0, -1)), 0.9, r);
    if (t > 0.0) {
        vec3 N = vec3_unit_vector(vec3_subtraction(get_vec3_ptr(make_vec3(0,0,-1)), get_vec3_ptr(ray_at(r, t))));
        color pixel_color = vec3_scalar_multiply(0.5, get_vec3_ptr(make_vec3(N.e[0] + 1, N.e[1] + 1, N.e[2] + 1)));
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