#pragma once
#include <cstdint>

struct GrayscaleGPUContext {
    uint8_t* d_input = nullptr;
    uint8_t* d_output = nullptr;
    int width = 0;
    int height = 0;
};