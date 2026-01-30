#pragma once 
#include <cstdint>

__global__ void grayscale_kernel(
    const uint8_t* input,
    uint8_t* output,
    int width,
    int height
);