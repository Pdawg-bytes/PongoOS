#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "vector.h"

#pragma endregion

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#pragma region

#define GAMMA 2.2

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
#pragma region

uint32_t color_raw(Vector3 color) {
    uint8_t red = (uint8_t)(255.99 * color.x);
    uint8_t green = (uint8_t)(255.99 * color.y);
    uint8_t blue = (uint8_t)(255.99 * color.z);

    return (0xFFU << 24) | ((uint32_t)red << 16) | ((uint32_t)green << 8) | (uint32_t)blue;
}

#pragma endregion
