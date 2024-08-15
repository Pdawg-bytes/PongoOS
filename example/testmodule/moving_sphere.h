#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "common.h"
#include "hittable.h"

typedef struct moving_sphere
{
	point3 center_0;
	point3 center_1;
	double time_0;
	double time_1;
	double radius;
} moving_sphere;

hittable_object moving_sphere_create(point3 cen_0, point3 cen_1, double time_0, double time_1, double radius, material material)
{
	hittable_object object;
	moving_sphere* sphere;

	object.geometry = moving_sphere_geometry;
	object.material = material;
	object.pointer = malloc(sizeof(moving_sphere));
	if ((sphere = object.pointer))
	{
		sphere->center_0 = cen_0;
		sphere->center_1 = cen_1;
		sphere->time_0 = time_0;
		sphere->time_1 = time_1;
		sphere->radius = radius;
	}
	return (object);
}

static point3 moving_sphere_center(moving_sphere* sphere, double time)
{
	point3 center;

	center = vec3_subtract(sphere->center_1, sphere->center_0);
	vec3_multiply_scalar_update(&center, (time - sphere->time_0) / (sphere->time_1 - sphere->time_0));
	vec3_add_update(&center, sphere->center_0);
	return (center);
}

int moving_sphere_hit(moving_sphere* sphere, ray* r, double t_min, double t_max, hit_record* rec)
{
	point3 cen = moving_sphere_center(sphere, r->time);
	vec3 oc = vec3_subtract(r->origin, cen);
	double a = vec3_length_squared(r->direction);
	double half_b = vec3_dot(oc, r->direction);
	double c = vec3_length_squared(oc) - sphere->radius * sphere->radius;

	double discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
		return (FALSE);
	double sqrtd = square_root(discriminant);

	double root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root)
	{
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return (FALSE);
	}

	rec->t = root;
	rec->p = ray_at(r, rec->t);
	vec3 outward_normal = vec3_divide_scalar(vec3_subtract(rec->p, cen), sphere->radius);
	set_face_normal(rec, r, outward_normal);

	return (TRUE);
}

#endif