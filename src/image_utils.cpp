#include "image_utils.hpp"
#include <cstdlib>
#include <cstring>

Image create_image(int width, int height, int channels){
    Image img;
    img.width = width;
    img.height = height;
    img.channels = channels;

    size_t size = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(channels);
    img.data = static_cast<uint8_t*>(std::malloc(size));

    std::memset(img.data, 0, size);

    return img;
}

void free_image(Image& img){
    if (img.data){
        std::free(img.data);
        img.data = nullptr;
    }
}