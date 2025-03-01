#include <pongo.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "common.h"
#include "rt.h"

#include "material.h"
#include "hittable_list.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "camera.h"

#define lambertian_create(c) material_create(lambertian, c, FALSE, FALSE); 
#define metal_create(c, f) material_create(metal, c, f, FALSE); 
#define dielectric_create(c, i) material_create(dielectric, c, FALSE, i);
#define emitter_create(c) material_create(emitter, c, FALSE, FALSE);


uint32_t color_raw(color color, int samples_per_pixel) {
	double r = color.x;
	double g = color.y;
	double b = color.z;

	double scale = 1.0 / (double)samples_per_pixel;
	r = square_root(scale * r);
	g = square_root(scale * g);
	b = square_root(scale * b);

    uint8_t ir = (uint8_t)(256 * clamp(r, 0.0, 0.999));
    uint8_t ig = (uint8_t)(256 * clamp(g, 0.0, 0.999));
    uint8_t ib = (uint8_t)(256 * clamp(b, 0.0, 0.999));

    return (0xFFU << 24) | ((uint32_t)ir << 16) | ((uint32_t)ig << 8) | (uint32_t)ib;
}

hittable_list* random_scene()
{
	color albedo;
	hittable_list* world = NULL;
	int a, b;

	material ground_material = lambertian_create(color_(0.5, 0.5, 0.5));
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(0, -1000, 0), 1000, ground_material)));

	for (a = -11; a < 11; a++)
	{
		for (b = -11; b < 11; b++)
		{
			double choose_mat = random_double();
			point3 center = point3_(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((vec3_length(vec3_subtract(center, point3_(4, 0.2, 0))) > 0.9))
			{
				material sphere_material;

				if (choose_mat > 0.65)
				{
					albedo = vec3_random_minmax(0.5, 1);
					sphere_material = emitter_create(albedo);
					hittable_list_push(&world, hittable_list_create(sphere_create(center, 0.2, sphere_material)));
				}
				else if (choose_mat < 0.8)
				{
					albedo = vec3_multiply(vec3_random(), vec3_random());
					sphere_material = lambertian_create(albedo);
					// Uncomment these lines to have a few "moving spheres" in your render.
					// point3 center_ = vec3_add(center, vec3_create(0, random_double_minmax(0, 0.5), 0));
					// hittable_list_push(&world, hittable_list_create(moving_sphere_create(center, _center, 0.0, 1.0, 0.2, sphere_material)));
					hittable_list_push(&world, hittable_list_create(sphere_create(center, 0.2, sphere_material)));
				}
				else
				{
					if (choose_mat < 0.95)
					{
						albedo = vec3_random_minmax(0.5, 1);
						double fuzz = random_double_minmax(0, 0.5);
						sphere_material = metal_create(albedo, fuzz);
						hittable_list_push(&world, hittable_list_create(sphere_create(center, 0.2, sphere_material)));
					}
					else
					{
						sphere_material = dielectric_create(color_(1, 1, 1), 1.5);
						hittable_list_push(&world, hittable_list_create(sphere_create(center, 0.2, sphere_material)));
					}
				}
			}
		}
	}

	material material0 = emitter_create(vec3_create(0.4, 0.14, 0.72));
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(-8, 1, 0), 1, material0)));

	material material1 = dielectric_create(color_(1, 1, 1), 1.5);
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(0, 1, 0), 1, material1)));

	material material2 = lambertian_create(color_(0.4, 0.2, 0.1));
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(-4, 1, 0), 1, material2)));

	material material3 = metal_create(color_(0.7, 0.6, 0.5), 0);
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(4, 1, 0), 1, material3)));

	return (world);
}

hittable_list* cornell_box()
{
	hittable_list* world = NULL;

	material light_material = emitter_create(color_(10, 10, 10));
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(0, 24.5, -38), 15, light_material)));

	material ground_material = lambertian_create(color_(0.5, 0.5, 0.5));
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(0, -1010, -20), 1000, ground_material)));

	material ceiling_material = lambertian_create(color_(0.5, 0.5, 0.5));
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(0, 1010, -20), 1000, ceiling_material)));

	material left_wall_material = lambertian_create(color_(1, 0.2, 0.2));
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(-1010, 0, -20), 1000, left_wall_material)));

	material right_wall_material = lambertian_create(color_(0.2, 1, 0.2));
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(1010, 0, -20), 1000, right_wall_material)));

	material front_wall_material = lambertian_create(color_(0.5, 0.5, 0.5));
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(0, 0, -1055), 1000, front_wall_material)));

	material back_wall_material = lambertian_create(color_(0.5, 0.5, 0.5));
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(0, 0, 1030), 1000, back_wall_material)));


	material smooth_sphere_material = metal_create(color_(0.5, 0.5, 0.5), 0.1);
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(-4, -7, -40), 3, smooth_sphere_material)));

	material glass_sphere_material = dielectric_create(color_(1, 1, 1), 1.5);
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(4, -7, -34), 3, glass_sphere_material)));

	material lambertian_sphere_material = lambertian_create(color_(0.2, 0.2, 1));
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(4.5, 4.5, -38), 2, lambertian_sphere_material)));

	return (world);
}

hittable_list* bubble_scene()
{
	hittable_list* world = NULL;

	material ground_material = lambertian_create(color_(0.8, 0.8, 0.0));
	material center_material = lambertian_create(color_(0.1, 0.2, 0.5));
	material left_material = dielectric_create(color_(1.0, 1.0, 1.0), 1.5);
	material bubble_material = dielectric_create(color_(1.0, 1.0, 1.0), 1.00 / 1.50);
	material right_material = metal_create(color_(0.8, 0.6, 0.2), 1.0);

	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(0.0, -100.5, -1.0), 100.0, ground_material)));
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(0.0, 0.0, -1.2), 0.5, center_material)));
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(-1.0, 0.0, -1.0), 0.5, left_material)));
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(-1.0, 0.0, -1.0), 0.4, bubble_material)));
	hittable_list_push(&world, hittable_list_create(sphere_create(point3_(1.0, 0.0, -1.0), 0.5, right_material)));

	return (world);
}

color ray_color(ray* r, hittable_list* world, int depth)
{
	hit_record rec;

	if (depth <= 0)
		return (color_(0, 0, 0));

	if (hittable_list_hit(world, r, 0.001, infinity_val, &rec))
	{
		ray scattered_ray;
		color attenuation;

		if (scatter(r, &rec, &attenuation, &scattered_ray))
		{
			if (vec3_length(scattered_ray.direction) == 0) {
				return attenuation;
			}
			return (vec3_multiply(attenuation, ray_color(&scattered_ray, world, depth - 1)));
		}
		return color_(0, 0, 0);
	}

	vec3 unit_direction = vec3_uv(r->direction);
	double t = 0.5 * (unit_direction.y + 1.0);
	return (vec3_add(vec3_multiply_scalar(color_(1.0, 1.0, 1.0), 1.0 - t), vec3_multiply_scalar(color_(0.5, 0.7, 1.0), t)));
	
	// Uncomment this line and comment the 3 above to remove the sky.
	// return vec3_create(0, 0, 0);
}


void main()
{
    const int image_height = (int)(IMAGE_WIDTH / (ASPECT_RATIO));
    int i, j, s;

	// Acts as a framebuffer cache for the output of our render.
    color* render_buffer = (color*)malloc(IMAGE_WIDTH * image_height * sizeof(color));

    hittable_list* world = bubble_scene();

	// Camera position for Bubble scene
	point3 look_from = point3_(-2, 2, 1);
	point3 look_at = point3_(0, 0, -1);

	// Camera position for Random Scene
	// point3 look_from = point3_(13, 2, 3);
	// point3 look_at = point3_(0, 0, 0);

	// Camera position for Cornell Box
    // point3 look_from = point3_(0, 0, 20);
    // point3 look_at = point3_(0, 0, 0);

    vec3 vec_upwards = vec3_create(0, 1, 0);

    camera cam = camera_create(look_from, look_at, vec_upwards, 20, ASPECT_RATIO, APERTURE, DIST_TO_FOCUS, 0, 1);

    puts("Render started.\n");

    for (j = image_height - 1; j >= 0; --j)
    {
		fprintf(stderr, "\rScanlines remaining: %d ", j), fflush(stderr);
        for (i = 0; i < IMAGE_WIDTH; ++i)
        {
            color pixel_color = color_(0, 0, 0);
            for (s = 0; s < MAX_SAMPLES_PER_PIXEL; ++s)
            {
                double u = ((double)i + random_double()) / (IMAGE_WIDTH - 1);
                double v = ((double)j + random_double()) / (image_height - 1);
                ray r = camera_get_ray(&cam, u, v);
                vec3_add_update(&pixel_color, ray_color(&r, world, MAX_RAY_DEPTH));
            }
            render_buffer[j * IMAGE_WIDTH + i] = pixel_color;
        }
    }

	// We write to the framebuffer multiple times to avoid corruption.
	int pass = 0;
	while (pass <= 5)
	{
    	for (j = 0; j < image_height; ++j)
    	{
        	for (i = 0; i < IMAGE_WIDTH; ++i)
        	{
				// We flip our image vertically because the Pongo framebuffer writes backwards.
            	int flipped_j = image_height - 1 - j;

            	color pixel_color = render_buffer[flipped_j * IMAGE_WIDTH + i];

            	uint32_t framebufferColor = color_raw(pixel_color, MAX_SAMPLES_PER_PIXEL);
            	size_t framebufferOffset = (j + IMAGE_OFFSET) * gRowPixels + i;
            	gFramebuffer[framebufferOffset] = framebufferColor;
        	}
    	}
    	pass++;
	}

    hittable_list_clear(&world);
    free(render_buffer); // Don't forget to free the allocated memory for the buffer cache!

    puts("\n\nRender complete.");
}