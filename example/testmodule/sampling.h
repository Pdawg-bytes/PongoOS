#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "vector.h"
#include "math_tools.h"

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
#pragma region

Vector3 uniform_sample_on_hemisphere(double u1, double u2) {
	const double sin_theta = square_root(fmaximum(0.0, 1.0 - u1 * u1));
	const double phi = 2.0 * PI * u2;
	const Vector3 result = { 
		cosine(phi) * sin_theta, 
		sine(phi) * sin_theta, 
		u1 
	};
	return result;
}

Vector3 cosine_weighted_sample_on_hemisphere(double u1, double u2) {
	const double cos_theta = square_root(1.0 - u1);
	const double sin_theta = square_root(u1);
	const double phi = 2.0 * PI * u2;
	const Vector3 result = { 
		cosine(phi) * sin_theta, 
		sine(phi) * sin_theta, 
		cos_theta 
	};
	return result;
}

#pragma endregion