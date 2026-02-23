#include "gaussian_gpu.hpp"
#include <cuda_runtime.h>

//forward declaration of kernel
__global__ void gaussian_blur_3x3(
    const uint8_t* input,
    uint8_t* output,
    int width,
    int height
);

void gaussian_gpu_init(GaussianGPUContext& ctx, int width, int height) {
    ctx.width = width;
    ctx.height = height;

    size_t size = static_cast<size_t>(width) * height;

    cudaMalloc(&ctx.d_input, size);
    cudaMalloc(&ctx.d_output, size);
}

void gaussian_gpu_run(GaussianGPUContext& ctx, const uint8_t* h_input, uint8_t* h_output) {
    size_t size = static_cast<size_t>(ctx.width) *ctx.height;

    cudaMemcpy(ctx.d_input, h_input, size, cudaMemcpyHostToDevice);
    dim3 block(16,16);
    dim3 grid( (ctx.width + block.x - 1)/block.x, (ctx.height + block.y - 1) / block.y);

    gaussian_blur_3x3<<<grid,block>>>(
        ctx.d_input, ctx.d_output,
        ctx.width, ctx.height
    );

    cudaDeviceSynchronize();

    cudaMemcpy(h_output, ctx.d_output, size, cudaMemcpyDeviceToHost);
}

void gaussian_gpu_destroy(GaussianGPUContext& ctx){
    if(ctx.d_input) cudaFree(ctx.d_input);
    if(ctx.d_output) cudaFree(ctx.d_output);

    ctx.d_input = nullptr;
    ctx.d_output = nullptr;
}