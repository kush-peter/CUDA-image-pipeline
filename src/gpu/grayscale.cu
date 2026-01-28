#include <cuda_runtime.h>
#include "image.hpp"

__global__ void grayscale_kernel(const uint8_t* input, uint8_t* output, int width, int height){
    //thread mapping
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    //bound check
    if(x >= width || y >= height) return;

    int rgb_idx = (y * width + x) * 3;
    int gray_idx = (y * width + x) * 1;
    
    uint8_t b = input[rgb_idx + 0];
    uint8_t g = input[rgb_idx + 1];
    uint8_t r = input[rgb_idx + 2];

    float gray = 0.299f * r + 0.587f * g + 0.114f * b;
    output[gray_idx] = static_cast<uint8_t>(gray);

}

void grayscale_gpu(const Image& input, Image& output){
    uint8_t* d_input = nullptr;
    uint8_t* d_output = nullptr;

    size_t input_size = static_cast<size_t>(input.width) * static_cast<size_t>(input.height) * 3;
    size_t output_size = static_cast<size_t>(output.width) * static_cast<size_t>(output.height);

    cudaMalloc(&d_input, input_size);
    cudaMalloc(&d_output, output_size);

    cudaMemcpy(d_input, input.data, input_size, cudaMemcpyHostToDevice);

    dim3 block(16,16);
    dim3 grid(
        (input.width + block.x - 1) / block.x,
        (input.height + block.y - 1) / block.y
    );

    grayscale_kernel<<<grid, block>>>(
        d_input,
        d_output,
        input.width,
        input.height
    );

    cudaDeviceSynchronize();

    cudaMemcpy(output.data, d_output, output_size, cudaMemcpyDeviceToHost);

    cudaFree(d_input);
    cudaFree(d_output);

}