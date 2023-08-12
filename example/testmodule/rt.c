#include <pongo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "rt.h"

double fmax_impl(double a, double b) {
    return (a > b) ? a : b;
}

uint32_t color_raw(color color) {
    uint8_t red = (uint8_t)(255.99 * color.e[0]);
    uint8_t green = (uint8_t)(255.99 * color.e[1]);
    uint8_t blue = (uint8_t)(255.99 * color.e[2]);

    return (0xFFU << 24) | ((uint32_t)red << 16) | ((uint32_t)green << 8) | (uint32_t)blue;
}

void main() {
    int image_width = 754;
    int image_height = 754;

    hittable_list* world = hittable_list_create();

    // fix this tomorrow its 2 in the morning
    hittable* sphere1 = sphere_create(make_vec3(0, 0, -1), 0.5);
    hittable* sphere2 = sphere_create(make_vec3(0, -100.5, -1), 100);

    hittable_list_add(world, sphere1);
    hittable_list_add(world, sphere2);

    float focal_length = 1.0;
    float viewport_height = 2.0;
    float viewport_width = 2.0;

    point3 camera_center = make_vec3(0, 0, -3);

    vec3 viewport_u = make_vec3(viewport_width, 0, 0);
    vec3 viewport_v = make_vec3(0, -viewport_height, 0);

    vec3 horizontal = vec3_scalar_multiply(viewport_width, &viewport_u);
    vec3 vertical = vec3_scalar_multiply(viewport_height, &viewport_v);

    vec3 lower_left_corner = vec3_subtraction(&camera_center, &horizontal);
    lower_left_corner = vec3_subtraction(&lower_left_corner, &vertical);
    lower_left_corner = vec3_scalar_multiply(0.5, &lower_left_corner);

    vec3 pixel_delta_u = vec3_scalar_divide(&horizontal, image_width);
    vec3 pixel_delta_v = vec3_scalar_divide(&vertical, image_height);

    int pass = 0;
    while (pass <= 5) {
        for (int j = image_height - 1; j >= 0; j--) {
            for (int i = 0; i < image_width; i++) {
                vec3 pixel_center_temp1 = vec3_scalar_multiply(i, &pixel_delta_u);
                vec3 pixel_center_temp2 = vec3_scalar_multiply(j, &pixel_delta_v);
                vec3 pixel_center_temp3 = vec3_addition(&pixel_center_temp1, &pixel_center_temp2);
                vec3 pixel_center = vec3_addition(&lower_left_corner, &pixel_center_temp3);

                vec3 ray_direction = vec3_subtraction(&pixel_center, &camera_center);
                ray r = make_ray(&camera_center, &ray_direction);
                color pixel_color = ray_color(&r, world);

                uint32_t framebufferColor = color_raw(pixel_color);
                size_t framebufferOffset = (j + 200) * gRowPixels + i; 
                gFramebuffer[framebufferOffset] = framebufferColor;
            }
        }
        pass++;
    }
    puts("Render complete.");
}
