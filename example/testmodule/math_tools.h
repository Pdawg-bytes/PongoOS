#pragma once

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#pragma endregion

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#pragma region

#define PI 3.14159265358979323846
#define infinity_val INFINITY
#define RAND_MAX 0x7fff

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
#pragma region

double clamp(double x, double low, double high) {
	return (x < high) ? ((x > low) ? x : low) : high;
}

double power(double base, int exponent) {
    if (exponent == 0) {
        return 1.0;
    }
    
    double result = base;
    int absExponent = (exponent < 0) ? -exponent : exponent;
    
    for (int i = 1; i < absExponent; i++) {
        result *= base;
    }
    
    return (exponent < 0) ? 1.0 / result : result;
}

double ldexponent(double x, int exp) {
    return x * power(2.0, (double)exp);
}

uint8_t to_byte(double x, double gamma) {
	return (uint8_t)clamp(255.0 * power(x, 1.0 / gamma), 0.0, 255.0);
}

float fabsolute(float x) {
    if(x<0) {
        return -x;
    }
    return x;
}

int minimum(int x, int y) {
    return (x < y) ? x : y;
}

int maximum(int x, int y) {
    return (x > y) ? x : y;
}

double fmaximum(double x, double y) {
    return (x > y) ? x : y;
}

double fminimum(double x, double y) {
    return (x < y) ? x : y;
}

float sine(float x) {
    bool neg = false;

    if (x > PI) {
        neg = true;
        x -= PI;
    }

    if (x < 0) {
        neg = true;
    }

    if (x < -PI) {
        neg = false;
        x += PI;
    }

    float x_ = fabsolute(x);
    const float B = 4 / PI;
    const float C = -4 / (PI * PI);

    float ret = B * x_ + C * x_ * x_;

    return neg ? -ret : ret;
}

float cosine(float x) {
    return x > PI/2 ? sine(PI/2 - x) : sine(PI/2 + x);
}

float tangent(float x) {
    return sine(x)/cosine(x);
}

double square_root(double n){
  double lo = minimum(1, n), hi = maximum(1, n), mid;

  while(100 * lo * lo < n) lo *= 10;
  while(0.01 * hi * hi > n) hi *= 0.1;

  for(int i = 0 ; i < 100 ; i++){
      mid = (lo+hi)/2;
      if(mid*mid == n) return mid;
      if(mid*mid > n) hi = mid;
      else lo = mid;
  }
  return mid;
}

double ceiling(double x) {
    if (x >= 0.0) {
        int intPart = (int)x;
        if (x == (double)intPart) {
            return x;
        } else {
            return (double)(intPart + 1);
        }
    } else {
        int intPart = (int)(x - 1.0);
        return (double)(intPart + 1);
    }
}

double floor_impl(double x) {
    if (x >= 0.0) {
        int intPart = (int)x;
        return (double)intPart;
    } else {
        int intPart = (int)(x - 1.0);
        return (double)(intPart + 1);
    }
}

double truncate(double x) {
    if (x >= 0.0) {
        return (double)((int)x);
    } else {
        return (double)((int)x + 1);
    }
}

double round_impl(double x) {
    if (x >= 0.0) {
        return floor_impl(x + 0.5);
    } else {
        return ceiling(x - 0.5);
    }
}

static unsigned long next = 1;
int rand_impl(void) {
    next = next * 214013L + 2531011L;
    return (int)((next >> 16) & 0x7FFF);
}

#pragma endregion