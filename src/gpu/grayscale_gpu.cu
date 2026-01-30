#include <cuda_runtime.h>
#include <stdexcept>

#include "gpu_context.hpp"
#include "image.hpp"
#include "grayscale_kernel.hpp"

void grayscale_gpu_init(GrayscaleGPUContext& ctx, int width, int height) {
    ctx.width = width;
    ctx.height = height;

    size_t input_size = static_cast<size_t>(width) * static_cast<size_t>(height) * 3;
    size_t output_size = static_cast<size_t>(width) * static_cast<size_t>(height);

    cudaMalloc(&ctx.d_input, input_size);
    cudaMalloc(&ctx.d_output, output_size);

    if (!ctx.d_input || !ctx.d_output) {
        throw std::runtime_error("Failed to allocate memory in GPU");
    }
}

void grayscale_gpu_destroy(GrayscaleGPUContext& ctx) {
    if(ctx.d_input){
        cudaFree(ctx.d_input);
        ctx.d_input = nullptr;
    }
    if(ctx.d_output){
        cudaFree(ctx.d_output);
        ctx.d_output = nullptr;
    }

    ctx.width = 0;
    ctx.height = 0;
}

void grayscale_gpu_run(GrayscaleGPUContext& ctx, const Image& input, Image& output){
    if(input.width != ctx.width || input.height != ctx.height){
        throw std::runtime_error("Input image size mismatch with GPU context");
    }
    if(output.width != ctx.width || output.height != ctx.height){
        throw std::runtime_error("Output image size mismatch with GPU context");
    }

    size_t input_size = static_cast<size_t>(ctx.width) * static_cast<size_t>(ctx.height) * 3;

    cudaMemcpy(ctx.d_input, input.data, input_size, cudaMemcpyHostToDevice);

    dim3 block(16,16);
    dim3 grid( (ctx.width + block.x - 1) / block.x, (ctx.height + block.y - 1) / block.y);

    grayscale_kernel <<<grid,block>>>(
        ctx.d_input,
        ctx.d_output,
        ctx.width,
        ctx.height
    );

    cudaDeviceSynchronize();

    size_t output_size = static_cast<size_t>(ctx.width) * static_cast<size_t>(ctx.height);

    cudaMemcpy(output.data, ctx.d_output, output_size, cudaMemcpyDeviceToHost);
}