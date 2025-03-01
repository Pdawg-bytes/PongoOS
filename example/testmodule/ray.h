#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct ray
{
	point3 origin;
	vec3 direction;
	double time;
} ray;

ray ray_create(point3 origin, vec3 direction, double time)
{
	ray r;

	r.origin = origin;
	r.direction = direction;
	r.time = time;
	return (r);
}

point3 ray_at(ray* r, double t)
{
	return (vec3_add(r->origin, vec3_multiply_scalar(r->direction, t)));
}

#endif