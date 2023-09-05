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

double square_root(double x) {
    double y;
    int p,square,c;

    p = 0;
    do
    {
        p++;
        square = (p+1) * (p+1);
    }
    while( x > square );

    y = (double)p;
    c = 0;
    while(c<10)
    {
        y = (x/y + y)/2;
        if( y*y == x)
            return(y);
        c++;
    }
    return(y);
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

#pragma endregion