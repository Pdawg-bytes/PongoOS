#ifndef HITTABLE_H
#define HITTABLE_H

#include "common.h"
#include "material.h"

void set_face_normal(hit_record* rec, ray* r, vec3 outward_normal)
{
	rec->front_face = vec3_dot(r->direction, outward_normal) < 0;
	rec->hit_normal = rec->front_face ? outward_normal : vec3_negate(outward_normal);
}

typedef enum object_geometry
{
	sphere_geometry,
	moving_sphere_geometry

} geometry;

typedef struct hittable_object
{
	geometry geometry;
	material material;
	void* pointer;
} hittable_object;

#endif