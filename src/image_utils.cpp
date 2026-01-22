#include "image_utils.hpp"
#include <cstdlib>
#include <cstring>
#include <opencv2/opencv.hpp>

Image image_from_cvmat(const cv::Mat& mat){
    Image img;
    img.width = mat.cols;
    img.height = mat.rows;
    img.channels = mat.channels();

    size_t size = static_cast<size_t>(img.width) * static_cast<size_t>(img.height) * static_cast<size_t>(img.channels);

    img.data = static_cast<uint8_t*>(std::malloc(size));
    std::memcpy(img.data, mat.data, size);
    return img;
}

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

cv::Mat cvmat_from_image(const Image& img){
    int type = (img.channels == 3) ? CV_8UC3 : (img.channels == 1) ? CV_8UC1 : -1;

    if(type == -1){
        throw std::runtime_error("Unsupported channel count");
    }

    cv::Mat mat(img.height, img.width, type);
    std::memcpy(mat.data, img.data, static_cast<size_t>(img.width) * static_cast<size_t>(img.height) * static_cast<size_t>(img.channels));

    return mat;
}