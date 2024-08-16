#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"

typedef enum surface_type
{
	metal,
	lambertian,
	dielectric,
	emitter
} surface_type;

typedef struct material
{
	color albedo;
	surface_type surface;
	double fuzziness;
	double ir;
} material;

typedef struct hit_record
{
	point3 p;
	vec3 hit_normal;
	material material;
	double t;
	int	front_face;
} hit_record;

material material_create(surface_type surface, color surface_color, double fuzziness, double refractive_index)
{
	material m;
	m.surface = surface;
	m.albedo = surface_color;
	m.fuzziness = fuzziness;
	m.ir = refractive_index;
	return (m);
}

static double reflectance(double cosine, double ref_idx)
{
	double r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * power((1 - cosine), 5));
}

int scatter(ray* r_in, hit_record* rec, color* attenuation, ray* scattered)
{
	vec3 direction;
	vec3 unit_direction;
	double refraction_ratio;
	double cos_theta;
	double sin_theta;

	switch (rec->material.surface)
	{
		case lambertian:
			direction = vec3_add(rec->hit_normal, vec3_random_uv());

			if (vec3_near_zero(direction))
				direction = rec->hit_normal;
			break;

		case dielectric:
			refraction_ratio = rec->front_face ? (1.0 / rec->material.ir) : rec->material.ir;

			unit_direction = vec3_uv(r_in->direction);
			cos_theta = fminimum(vec3_dot(vec3_negate(unit_direction), rec->hit_normal), 1.0);
			sin_theta = square_root(1.0 - cos_theta * cos_theta);

			if (refraction_ratio * sin_theta > 1.0 || reflectance(cos_theta, refraction_ratio) > random_double())
				direction = vec3_reflect(vec3_uv(r_in->direction), rec->hit_normal);
			else
				direction = vec3_refract(unit_direction, rec->hit_normal, refraction_ratio);
			break;

		case metal:
			direction = vec3_reflect(vec3_uv(r_in->direction), rec->hit_normal);
			vec3_add_update(&direction, vec3_multiply_scalar(vec3_random_in_unit_sphere(), rec->material.fuzziness));
			break;

		case emitter:
			direction = vec3_create(0, 0, 0);
	}
	*scattered = ray_create(rec->p, direction, r_in->time);
	*attenuation = rec->material.albedo;

	return (rec->material.surface != metal || vec3_dot(scattered->direction, rec->hit_normal) > 0);
}

#endif