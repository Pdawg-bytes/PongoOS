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
    int image_height = 424;

    float focal_length = 1.0;
    float viewport_height = 2.0;
    float viewport_width = 2.0;

    point3 camera_center = make_vec3(0, 0, 0);

    vec3 viewport_u = make_vec3(viewport_width, 0, 0);
    vec3 viewport_v = make_vec3(0, -viewport_height, 0);

    vec3 pixel_delta_u = vec3_scalar_divide(&viewport_u, image_width);
    vec3 pixel_delta_v = vec3_scalar_divide(&viewport_v, image_height);

    vec3 viewport_upper_left = vec3_subtraction(&camera_center, &viewport_u);
    vec3 upper_left_focal = make_vec3(0, 0, focal_length);
    viewport_upper_left = vec3_subtraction(&viewport_upper_left, get_vec3_ptr(upper_left_focal));
    vec3 pixel00_loc_temp = vec3_addition(&pixel_delta_u, &pixel_delta_v);
    vec3 pixel00_loc_temp2 = vec3_scalar_multiply(0.5, &pixel00_loc_temp);
    vec3 pixel00_loc = vec3_subtraction(&viewport_upper_left, &pixel00_loc_temp2);

    int pass = 0;
    while (pass <= 5) {
        for (int j = image_height - 1; j >= 0; j--) {
            for (int i = 0; i < image_width; i++) {
                vec3 pixel_center_temp1 = vec3_scalar_multiply(i, &pixel_delta_u);
                vec3 pixel_center_temp2 = vec3_scalar_multiply(j, &pixel_delta_v);
                vec3 pixel_center_temp3 = vec3_addition(&pixel_center_temp1, &pixel_center_temp2);
                vec3 pixel_center = vec3_addition(&pixel00_loc, &pixel_center_temp3);
            
                vec3 ray_direction = vec3_subtraction(&pixel_center, &camera_center);
                ray r = make_ray(&camera_center, &ray_direction);
                color pixel_color = ray_color(&r);

                uint32_t framebufferColor = color_raw(pixel_color);
                size_t framebufferOffset = (j + 200) * gRowPixels + i; 
                gFramebuffer[framebufferOffset] = framebufferColor;
            }
        }
        pass++;
    }
    puts("Render complete.");
}
