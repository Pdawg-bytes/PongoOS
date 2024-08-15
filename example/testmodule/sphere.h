#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "hittable.h"

typedef struct sphere
{
	point3 center;
	double radius;
} sphere;

hittable_object sphere_create(point3 center, double radius, material material)
{
	hittable_object h;
	sphere* s;

	h.geometry = sphere_geometry;
	h.material = material;
	h.pointer = malloc(sizeof(sphere));
	if ((s = h.pointer))
	{
		s->center = center;
		s->radius = radius;
	}
	return (h);
}

int sphere_hit(sphere* s, ray* r, double t_min, double t_max, hit_record* rec)
{
	vec3 oc = vec3_subtract(r->origin, s->center);
	double a = vec3_length_squared(r->direction);
	double half_b = vec3_dot(oc, r->direction);
	double c = vec3_length_squared(oc) - s->radius * s->radius;

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
	vec3 outward_normal = vec3_divide_scalar(vec3_subtract(rec->p, s->center), s->radius);
	set_face_normal(rec, r, outward_normal);

	return (TRUE);
}

#endif