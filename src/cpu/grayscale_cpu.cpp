#include "image_utils.hpp"
#include <stdexcept>

void grayscale_cpu(const Image& input, Image& output){
    if(input.channels != 3 || output.channels != 1){
        throw std::runtime_error("Invalid error format for grayscale!");
    }
    if(input.width != output.width || input.height != output.height){
        throw std::runtime_error("Input and output image size mismatch!");
    }

    int width = input.width;
    int height = input.height;

    for(int y = 0; y < height; ++y){
        for(int x = 0; x < width; ++x){
            int rgb_idx = (y*width+x) * 3;
            int gray_idx = (y*width+x) * 1;

            uint8_t r = input.data[rgb_idx + 0];
            uint8_t g = input.data[rgb_idx + 1];
            uint8_t b = input.data[rgb_idx + 2];

            float gray_f = 0.3f * r + 0.6f * g + 0.1f * b;

            output.data[gray_idx] = static_cast<uint8_t>(gray_f);
        }
    }
}