#ifndef HITTABLE_H
#define HITTABLE_H

#include "common.h"

typedef struct {
    point3 p;
    vec3 normal;
    double t;
    bool front_face;
} hit_record;

typedef struct hittable hittable;

typedef bool (*hit_function)(const hittable* obj, const ray* r, double ray_tmin, double ray_tmax, hit_record* rec);

struct hittable {
    hit_function hit;
};

static inline void set_face_normal(hit_record* rec, const ray* r, const vec3* outward_normal) {
    rec->front_face = vec3_dot(&r->dir, outward_normal) < 0;

    if(rec->front_face) {
        rec->normal = make_vec3(outward_normal->e[0], outward_normal->e[1], outward_normal->e[2]);
    } 
    else {
        rec->normal = vec3_neg(outward_normal);
    }
}

#endif