#include <iostream>
#include <opencv2/opencv.hpp>
#include "image_utils.hpp"

int main(){

    cv::Mat input = cv::imread("test.png");

    if (input.empty()){
        std::cout << "Failed to load image\n OHHHH SHIAATTTT";
        return 1;
    }
    //convert OCV image to my struct Image
    Image rgb = image_from_cvmat(input);
    std::cout << "Loaded image: " << rgb.width << "x" << rgb.height << " channels = " << rgb.channels << "\n";

    Image gray = create_image(rgb.width, rgb.height, 1); //allocate output 

    //CPU Grayscale
    grayscale_cpu(rgb, gray);

    //convert back to ocv for saving (imwrite)
    cv::Mat output = cvmat_from_image(gray);
    cv::imwrite("gray_cpu.png", output);

    //cleanup
    free_image(rgb);
    free_image(gray);

    std::cout << "Saved gray_cpu.png\n";
    return 0;

}