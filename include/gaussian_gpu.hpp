#pragma once
#include <cstdint>

struct GaussianGPUContext {
    uint8_t* d_input = nullptr;
    uint8_t* d_output = nullptr;

    int width = 0;
    int height = 0;
};

void gaussian_gpu_init(GaussianGPUContext& ctx, int width, int height);
void gaussian_gpu_run(GaussianGPUContext& ctx, const uint8_t* h_input, uint8_t* h_output);
void gaussian_gpu_destroy(GaussianGPUContext& ctx);

