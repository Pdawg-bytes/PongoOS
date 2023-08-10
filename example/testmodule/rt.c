#include <pongo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "rt.h"

typedef struct {
    double x, y, z;
} Vec3;

typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;

typedef struct {
    double r, g, b;
} Color;

typedef struct {
    Vec3 center;
    double radius;
    Color color;
} Sphere;

float square_root(float x) {
    if (x <= 0) {
        return 0;
    }

    float guess = x;
    float epsilon = 0.00001;

    while ((guess * guess - x) > epsilon) {
        guess = (guess + x / guess) / 2;
    }

    return guess;
}

double fmax_impl(double a, double b) {
    return (a > b) ? a : b;
}

uint32_t color_raw(Color color) {
    uint8_t red = (uint8_t)(255.99 * color.r);
    uint8_t green = (uint8_t)(255.99 * color.g);
    uint8_t blue = (uint8_t)(255.99 * color.b);

    return (0xFFU << 24) | ((uint32_t)red << 16) | ((uint32_t)green << 8) | (uint32_t)blue;
}

Vec3 subtract(Vec3 v1, Vec3 v2) {
    Vec3 result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
    return result;
}

double dot(Vec3 v1, Vec3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3 normalize(Vec3 v) {
    double length = square_root(v.x * v.x + v.y * v.y + v.z * v.z);
    Vec3 result = {v.x / length, v.y / length, v.z / length};
    return result;
}

int hit_sphere(Sphere sphere, Ray ray, double *t) {
    Vec3 oc = subtract(ray.origin, sphere.center);
    double a = dot(ray.direction, ray.direction);
    double b = 2.0 * dot(oc, ray.direction);
    double c = dot(oc, oc) - sphere.radius * sphere.radius;
    double discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0) {
        return 0;
    }
    
    double sqrt_discriminant = square_root(discriminant);
    double t0 = (-b - sqrt_discriminant) / (2.0 * a);
    double t1 = (-b + sqrt_discriminant) / (2.0 * a);
    
    if (t0 > 0 && t0 < *t) {
        *t = t0;
        return 1;
    }
    
    if (t1 > 0 && t1 < *t) {
        *t = t1;
        return 1;
    }
    
    return 0;
}

Color trace(Ray ray, Sphere *spheres, int num_spheres) {
    Color background = {0.7, 0.7, 0.7};
    double t = INFINITY;
    int hit_object = -1;
    
    for (int i = 0; i < num_spheres; i++) {
        if (hit_sphere(spheres[i], ray, &t)) {
            hit_object = i;
        }
    }
    
    if (hit_object != -1) {
        Sphere sphere = spheres[hit_object];
        Vec3 hit_point = {ray.origin.x + ray.direction.x * t, ray.origin.y + ray.direction.y * t, ray.origin.z + ray.direction.z * t};
        Vec3 normal = normalize(subtract(hit_point, sphere.center));
        
        Vec3 light_direction = {0, 1, 1};
        double cos_theta = dot(normal, light_direction);
        cos_theta = fmax_impl(cos_theta, 0.0);
        
        Color shaded_color = {sphere.color.r * cos_theta, sphere.color.g * cos_theta, sphere.color.b * cos_theta};
        return shaded_color;
    }
    
    return background;
}

void main() {
    int width = 754;
    int height = 754;
    
    Vec3 camera = {0, 0, 0};
    
    Sphere spheres[] = {
        {{0, 0, -5}, 1.0, {0.8, 0.2, 0.2}},
        {{-1.5, -0.5, -4}, 0.5, {0.2, 0.8, 0.2}},
        {{1.5, -0.5, -4}, 0.6, {0.2, 0.5, 0.8}}
    };
    int num_spheres = sizeof(spheres) / sizeof(spheres[0]);
    
    int pass = 0;
    while (pass<=5) {
        for (int j = height - 1; j >= 0; j--) {
            for (int i = 0; i < width; i++) {
                double u = (double)i / (width - 1);
                double v = (double)j / (height - 1);
            
                Ray ray = {{camera.x, camera.y, camera.z}, {u - 0.5, v - 0.5, -1}};
            
                //Color color = trace(ray, spheres, num_spheres);

                Color color;
                color.r = (double)i / width - 1;
                color.g = (double)j / height - 1;
                color.b = 0;

                //int ir = (int)(255.99 * color.r);
                //int ig = (int)(255.99 * color.g);
                //int ib = (int)(255.99 * color.b);

                uint32_t framebufferColor = color_raw(color);
                size_t framebufferOffset = (j + 200) * gRowPixels + i; 
                gFramebuffer[framebufferOffset] = framebufferColor;
            }
        }
        pass++;
    }
    puts("Render complete.");
}
