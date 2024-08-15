#ifndef VEC3_H
# define VEC3_H

#include "math_tools.h"

# define TRUE	1
# define FALSE	0

typedef struct vec3
{
	double x;
	double y;
	double z;
} vec3;

vec3 vec3_create(double e1, double e2, double e3)
{
	vec3 v;

	v.x = e1;
	v.y = e2;
	v.z = e3;
	return (v);
}

vec3 vec3_random()
{
	return (vec3_create(random_double(), random_double(), random_double()));
}

vec3 vec3_random_minmax(double min, double max)
{
	return (vec3_create(random_double_minmax(min, max), random_double_minmax(min, max), random_double_minmax(min, max)));
}

void vec3_update(vec3* v, double e1, double e2, double e3)
{
	v->x = e1;
	v->y = e2;
	v->z = e3;
}

vec3 vec3_negate(vec3 v)
{
	return (vec3_create(-v.x, -v.y, -v.z));
}

int vec3_near_zero(vec3 v)
{
	double s = 1e-8;
	return ((fabsolute(v.x) < s) && (fabsolute(v.y) < s) && (fabsolute(v.z) < s));
}

vec3 vec3_multiply(vec3 v, vec3 u)
{
	return (vec3_create(v.x * u.x, v.y * u.y, v.z * u.z));
}

void vec3_multiply_scalar_update(vec3* v, double t)
{
	v->x *= t;
	v->y *= t;
	v->z *= t;
}

vec3 vec3_multiply_scalar(vec3 v, double t)
{
	return (vec3_create(v.x * t, v.y * t, v.z * t));
}

void vec3_divide_scalar_update(vec3* v, double t)
{
	vec3_multiply_scalar_update(v, 1 / t);
}

vec3 vec3_divide_scalar(vec3 v, double t)
{
	return (vec3_multiply_scalar(v, 1 / t));
}

double vec3_length_squared(vec3 v)
{
	return (v.x * v.x
		+ v.y * v.y
		+ v.z * v.z);
}

double vec3_length(vec3 v)
{
	return (square_root(vec3_length_squared(v)));
}

void vec3_add_update(vec3* u, vec3 v)
{
	vec3_update(u, u->x + v.x, u->y + v.y, u->z + v.z);
}

vec3 vec3_add(vec3 u, vec3 v)
{
	return (vec3_create(u.x + v.x, u.y + v.y, u.z + v.z));
}

void vec3_subtract_update(vec3* u, vec3 v)
{
	vec3_update(u, u->x - v.x, u->y - v.y, u->z - v.z);
}

vec3 vec3_subtract(vec3 u, vec3 v)
{
	return (vec3_create(u.x - v.x, u.y - v.y, u.z - v.z));
}

double vec3_dot(vec3 u, vec3 v)
{
	return (u.x * v.x
		+ u.y * v.y
		+ u.z * v.z);
}

vec3 vec3_cross(vec3 u, vec3 v)
{
	vec3 result;
	result.x = u.y * v.z - u.z * v.y;
	result.y = u.z * v.x - u.x * v.z;
	result.z = u.x * v.y - u.y * v.x;
	return result;
}

vec3 vec3_uv(vec3 v)
{
	return (vec3_divide_scalar(v, vec3_length(v)));
}

vec3 vec3_random_in_unit_sphere()
{
	while (TRUE)
	{
		vec3 p = vec3_random_minmax(-1, 1);
		if (vec3_length_squared(p) >= 1)
			continue;
		return (p);
	}
}

vec3 vec3_random_uv()
{
	return (vec3_uv(vec3_random_in_unit_sphere()));
}

vec3 vec3_random_in_unit_disk()
{
	while (TRUE)
	{
		vec3 p = vec3_create(random_double_minmax(-1, 1), random_double_minmax(-1, 1), 0);
		if (vec3_length_squared(p) >= 1)
			continue;
		return (p);
	}
}

vec3 vec3_reflect(vec3 v, vec3 n)
{
	return (vec3_subtract(v, vec3_multiply_scalar(n, 2 * vec3_dot(v, n))));
}

vec3 vec3_refract(vec3 uv, vec3 n, double etai_over_etat)
{
	double cos_theta = fminimum(vec3_dot(vec3_negate(uv), n), 1.0);
	vec3 r_out_perp = vec3_multiply_scalar(vec3_add(uv, vec3_multiply_scalar(n, cos_theta)), etai_over_etat);
	vec3 r_out_parallel = vec3_multiply_scalar(n, -square_root(fabsolute(1.0 - vec3_length_squared(r_out_perp))));
	return (vec3_add(r_out_perp, r_out_parallel));
}

vec3(*color_)(double e1, double e2, double e3) = vec3_create;
vec3(*point3_)(double e1, double e2, double e3) = vec3_create;

typedef vec3 point3;
typedef vec3 color;

#endif