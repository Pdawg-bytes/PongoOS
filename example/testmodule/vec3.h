#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdio.h>

float square_root(int number)
{
    int start = 0, end = number;
    int mid;
 
    float ans;
    while (start <= end) {
 
        mid = (start + end) / 2;
 
        if (mid * mid == number) {
            ans = mid;
            break;
        }

        if (mid * mid < number) {
            ans=start;
            start = mid + 1;
        }
        else {
            end = mid - 1;
        }
    }

    float increment = 0.1;
    for (int i = 0; i < 5; i++) {
        while (ans * ans <= number) {
            ans += increment;
        }
        ans = ans - increment;
        increment = increment / 10;
    }
    return ans;
}

typedef struct {
    double e[3];
} vec3;

vec3 make_vec3(double e0, double e1, double e2) {
    vec3 v;
    v.e[0] = e0;
    v.e[1] = e1;
    v.e[2] = e2;
    return v;
}

vec3* get_vec3_ptr(vec3 v) {
    vec3* ptr = &v;
    return ptr;
}

double vec3_x(const vec3 *v) { return v->e[0]; }
double vec3_y(const vec3 *v) { return v->e[1]; }
double vec3_z(const vec3 *v) { return v->e[2]; }

vec3 vec3_neg(const vec3 *v) {
    return make_vec3(-v->e[0], -v->e[1], -v->e[2]);
}

double vec3_get(const vec3 *v, int i) {
    return v->e[i];
}

void vec3_set(vec3 *v, int i, double val) {
    v->e[i] = val;
}

void vec3_add(vec3 *v, const vec3 *u) {
    v->e[0] += u->e[0];
    v->e[1] += u->e[1];
    v->e[2] += u->e[2];
}

vec3 vec3_multiply(const vec3 *u, const vec3 *v) {
    vec3 result;
    result.e[0] = u->e[0] * v->e[0];
    result.e[1] = u->e[1] * v->e[1];
    result.e[2] = u->e[2] * v->e[2];
    return result;
}

double vec3_length_squared(const vec3 *v) {
    return v->e[0] * v->e[0] + v->e[1] * v->e[1] + v->e[2] * v->e[2];
}

double vec3_length(const vec3 *v) {
    return square_root(vec3_length_squared(v));
}

// point3 is an alias for vec3, providing geometric clarity in the code.
typedef vec3 point3;

void vec3_print(const vec3 *v) {
    printf("%lf %lf %lf", v->e[0], v->e[1], v->e[2]);
}

vec3 vec3_addition(const vec3 *u, const vec3 *v) {
    return make_vec3(u->e[0] + v->e[0], u->e[1] + v->e[1], u->e[2] + v->e[2]);
}

vec3 vec3_subtraction(const vec3 *u, const vec3 *v) {
    return make_vec3(u->e[0] - v->e[0], u->e[1] - v->e[1], u->e[2] - v->e[2]);
}

vec3 vec3_scalar_multiply(double t, const vec3 *v) {
    return make_vec3(t * v->e[0], t * v->e[1], t * v->e[2]);
}

vec3 vec3_divide(vec3 *v, double t) {
    return vec3_scalar_multiply(1.0 / t, v);
}

vec3 vec3_scalar_divide(const vec3 *v, double t) {
    return vec3_scalar_multiply(1.0 / t, v);
}

double vec3_dot(const vec3 *u, const vec3 *v) {
    return u->e[0] * v->e[0] + u->e[1] * v->e[1] + u->e[2] * v->e[2];
}

vec3 vec3_cross(const vec3 *u, const vec3 *v) {
    return make_vec3(u->e[1] * v->e[2] - u->e[2] * v->e[1],
                     u->e[2] * v->e[0] - u->e[0] * v->e[2],
                     u->e[0] * v->e[1] - u->e[1] * v->e[0]);
}

vec3 vec3_unit_vector(vec3 v) {
    double len = vec3_length(&v);
    return vec3_scalar_divide(&v, len);
}

#endif