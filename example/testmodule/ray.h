#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct {
    point3 orig;
    vec3 dir;
} ray;

ray make_ray(const point3 *origin, const vec3 *direction) {
    ray r;
    r.orig = *origin;
    r.dir = *direction;
    return r;
}

point3 ray_origin(const ray *r) {
    return r->orig;
}

vec3 ray_direction(const ray *r) {
    return r->dir;
}

point3 ray_at(const ray *r, double t) {
    vec3 scaled_dir = vec3_scalar_multiply(t, &r->dir);
    return vec3_addition(&r->orig, &scaled_dir);
}

#endif
