#pragma once

#include "image.hpp"

Image create_image(int width, int height, int channels);
void free_image(Image& img);