#pragma once 
#include <cstdint>

void grayscale_gpu_init(GrayscaleGPUContext& ctx, int width, int height);
void grayscale_gpu_destroy(GrayscaleGPUContext& ctx);
void grayscale_gpu_run(GrayscaleGPUContext& ctx, const Image& input, Image& output);