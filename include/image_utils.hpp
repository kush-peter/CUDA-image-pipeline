#pragma once

#include "image.hpp"

namespace cv {
    class Mat;
}
Image image_from_cvmat(const cv::Mat& mat);

Image create_image(int width, int height, int channels);
void free_image(Image& img);

cv::Mat cvmat_from_image(const Image& img);

void grayscale_cpu(const Image& input, Image& output);

void grayscale_gpu(const Image& input, Image& output);
