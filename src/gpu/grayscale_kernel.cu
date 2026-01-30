#include <cuda_runtime.h>
#include <cstdint>

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
