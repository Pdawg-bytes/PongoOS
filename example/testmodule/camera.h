#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

typedef struct camera
{
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 w;
	vec3 u;
	vec3 v;
	double lens_radius;
	double time_0, time_1;
} camera;

camera camera_create(point3 look_from, point3 look_at, vec3 vec_upwards, double vfov, double aspect_ratio, double aperture, double focus_distance, double time_0, double time_1)
{
	camera c;

	double theta = degrees_to_radians(vfov);
	double h = tangent(theta / 2);
	double viewport_height = 2.0 * h;
	double viewport_width = aspect_ratio * viewport_height;

	c.w = vec3_uv(vec3_subtract(look_from, look_at));
	c.u = vec3_uv(vec3_cross(vec_upwards, c.w));
	c.v = vec3_cross(c.w, c.u);

	c.origin = look_from;
	c.horizontal = vec3_multiply_scalar(c.u, viewport_width);
	vec3_multiply_scalar_update(&c.horizontal, focus_distance);
	c.vertical = vec3_multiply_scalar(c.v, viewport_height);
	vec3_multiply_scalar_update(&c.vertical, focus_distance);
	c.lower_left_corner = vec3_subtract(c.origin, vec3_divide_scalar(c.horizontal, 2));
	vec3_subtract_update(&c.lower_left_corner, vec3_divide_scalar(c.vertical, 2));
	vec3_subtract_update(&c.lower_left_corner, vec3_multiply_scalar(c.w, focus_distance));

	c.lens_radius = aperture / 2;
	c.time_0 = time_0;
	c.time_1 = time_1;
	
	return (c);
}

ray camera_get_ray(camera* cam, double s, double t)
{
	vec3 direction;
	vec3 random;
	vec3 offset;

	random = vec3_multiply_scalar(vec3_random_in_unit_disk(), cam->lens_radius);
	offset = vec3_multiply_scalar(cam->u, random.x);
	vec3_add_update(&offset, vec3_multiply_scalar(cam->v, random.y));

	direction = vec3_add(cam->lower_left_corner, vec3_multiply_scalar(cam->horizontal, s));
	vec3_add_update(&direction, vec3_multiply_scalar(cam->vertical, t));
	vec3_subtract_update(&direction, cam->origin);
	vec3_subtract_update(&direction, offset);
	return (ray_create(vec3_add(cam->origin, offset), direction, random_double_minmax(cam->time_0, cam->time_1)));
}

#endif