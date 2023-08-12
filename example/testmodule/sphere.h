#ifndef SPHERE_H
#define SPHERE_H

#include "common.h"

typedef struct {
    point3 center;
    double radius;
} sphere;

sphere sphere_create(point3 _center, double _radius) {
    sphere s;
    s.center = _center;
    s.radius = _radius;
    return s;
}

bool sphere_hit(const sphere* s, const ray* r, double ray_tmin, double ray_tmax, hit_record* rec) {
    vec3 sphere_center = s->center;
    vec3 oc = vec3_subtraction(&r->orig, &sphere_center);
    double a = vec3_length_squared(&r->dir);
    double half_b = vec3_dot(&oc, &r->dir);
    double sphere_radius = s->radius;
    double c = vec3_length_squared(&oc) - sphere_radius * sphere_radius;

    double discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    double sqrtd = sqrt(discriminant);

    double root = (-half_b - sqrtd) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
        root = (-half_b + sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root)
            return false;
    }

    rec->t = root;
    rec->p = ray_at(r, rec->t);
    vec3 hit_point = rec->p;
    vec3 subtracted_outward = vec3_subtraction(&hit_point, &sphere_center);
    vec3 outward_normal = vec3_divide(&subtracted_outward, &sphere_center);
    set_face_normal(rec, r, &outward_normal);

    return true;
}

#endif