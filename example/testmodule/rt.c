#include <pongo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "rtweekend.h"
#include "rt.h"

#include "material.h"
#include "hittable_list.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "camera.h"

#define make_lambertian(c) material_(lambertian, c, FALSE, FALSE); 
#define make_metal(c, f) material_(metal, c, f, FALSE); 
#define make_dielectric(i) material_(dielectric, color_(1,1,1), FALSE, i); 

uint32_t color_raw(color color, int samples_per_pixel) {
	double r = color.x;
    double g = color.y;
    double b = color.z;

    double scale = 1.0 / samples_per_pixel;
    r = square_root(scale * r);
    g = square_root(scale * g);
    b = square_root(scale * b);

    uint8_t ir = (uint8_t)(256 * clamp(r, 0.0, 0.999));
    uint8_t ig = (uint8_t)(256 * clamp(g, 0.0, 0.999));
    uint8_t ib = (uint8_t)(256 * clamp(b, 0.0, 0.999));

    return (0xFFU << 24) | ((uint32_t)ir << 16) | ((uint32_t)ig << 8) | (uint32_t)ib;
}

list* random_scene()
{
	color albedo;
	list* world = NULL;
	int a, b;

	material ground_material = make_lambertian(color_(0.5, 0.5, 0.5));
	push(&world, list_(sphere_(point3_(0, -1000, 0), 1000, ground_material)));

	for (a = 0; a < 1; a++)
	{
		for (b = 0; b < 11; b++)
		{
			double choose_mat = random_double();
			point3 center = point3_(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((length(subtract(center, point3_(4, 0.2, 0))) > 0.9))
			{
				material sphere_material;

				if (choose_mat < 0.8)
				{
					albedo = multiply__(vec3_random(), vec3_random());
					sphere_material = make_lambertian(albedo);
					//point3 center_ = add(center, vec3_(0, random_double_(0, 0.5), 0));
					push(&world, list_(sphere_(center, 0.2, sphere_material)));
				}
				else
				{
					if (choose_mat < 0.95)
					{
						albedo = vec3_random_(0.5, 1);
						double fuzz = random_double_(0, 0.5);
						sphere_material = make_metal(albedo, fuzz);
						push(&world, list_(sphere_(center, 0.2, sphere_material)));
					}
					else
					{
						sphere_material = make_dielectric(1.5);
						push(&world, list_(sphere_(center, 0.2, sphere_material)));
					}
				}
			}
		}
	}

	material material1 = make_dielectric(1.5);
	push(&world, list_(sphere_(point3_(0, 1, 0), 1, material1)));

	material material2 = make_lambertian(color_(0.4, 0.2, 0.1));
	push(&world, list_(sphere_(point3_(-4, 1, 0), 1, material2)));

	material material3 = make_metal(color_(0.7, 0.6, 0.5), 0);
	push(&world, list_(sphere_(point3_(4, 1, 0), 1, material3)));

	return (world);
}

color ray_color(ray* r, list* world, int depth)
{
	hit_record rec;

	if (depth <= 0)
		return (color_(0, 0, 0));

	if (hit(world, r, 0.001, infinity_val, &rec))
	{
		ray scattered;
		color attenuation;

		if (scatter(r, &rec, &attenuation, &scattered))
			return (multiply__(attenuation, ray_color(&scattered, world, depth - 1)));
		return (color_(0, 0, 0));
	}

	vec3 unit_direction = unit_vector(r->direction);
	double t = 0.5 * (unit_direction.y + 1.0);
	return (add(multiply(color_(1.0, 1.0, 1.0), 1.0 - t), multiply(color_(0.5, 0.7, 1.0), t)));
}

void main()
{
	const float aspect_ratio = 1/1;
	const int image_width = 754;
	const int image_height = (int)(image_width / aspect_ratio);
	const int samples_per_pixel = 10;
	const int max_depth = 10;
	int i, j, s;

	list* world = random_scene();

	point3 lookfrom = point3_(13, 2, 3);
	point3 lookat = point3_(0, 0, 0);
	vec3 vup = vec3_(0, 1, 0);
	double dist_to_focus = 10;
	double aperture = 0.1;

	camera cam = camera_(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0, 1);
	printf("camera setup complete\n");

	puts("Render started.\n");
	for (j = image_height - 1; j >= 0; --j)
	{
		for (i = 0; i < image_width; ++i)
		{
			color pixel_color = color_(0, 0, 0);
			for (s = 0; s < samples_per_pixel; ++s)
			{
				double u = ((double)i + random_double()) / (image_width - 1);
				double v = ((double)j + random_double()) / (image_height - 1);
				ray r = get_ray(&cam, u, v);
				add_(&pixel_color, ray_color(&r, world, max_depth));
			}

			uint32_t framebufferColor = color_raw(pixel_color, samples_per_pixel);
    		size_t framebufferOffset = (j + 200) * gRowPixels + i; 
            gFramebuffer[framebufferOffset] = framebufferColor;
		}
	}
	clear(&world);
	puts("Render complete.\n");
}