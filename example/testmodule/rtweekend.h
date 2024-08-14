#ifndef RTWEEKEND_H
# define RTWEEKEND_H

#include "erand48.h"
#include "math_tools.h"

double degrees_to_radians(double degrees)
{
	return (degrees * PI / 180.0);
}

double random_double()
{
	return (rand_impl() / (RAND_MAX + 1.0));
}

double random_double_(double min, double max)
{
	return (min + (max - min) * random_double());
}

# include "ray.h"
# include "vec3.h"

#endif