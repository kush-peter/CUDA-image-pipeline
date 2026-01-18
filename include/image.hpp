#pragma once
#include <cstdint>

struct Image
{
    int width;
    int height;
    int channels;
    uint8_t* data;
};
