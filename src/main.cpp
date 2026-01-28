#include <iostream>
#include <opencv2/opencv.hpp>
#include "image_utils.hpp"
#include "timer.hpp"

int main(){

    cv::Mat input = cv::imread("data/test_images/test_8k.jpg");

    if (input.empty()){
        std::cout << "Failed to load image\n OHHHH SHIAATTTT";
        return 1;
    }
    //convert OCV image to my struct Image
    Image rgb = image_from_cvmat(input);
    std::cout << "Loaded image: " << rgb.width << "x" << rgb.height << " channels = " << rgb.channels << "\n";

    Image gray = create_image(rgb.width, rgb.height, 1); //allocate output 
    
    Timer cpu_timer;

    cpu_timer.start();
    //CPU Grayscale
    grayscale_cpu(rgb, gray);

    double cpu_time_ms = cpu_timer.stop_ms();

    //convert back to ocv for saving (imwrite)
    cv::Mat output = cvmat_from_image(gray);
    cv::imwrite("data/result_images/gray_cpu.png", output);

    //cleanup
    

    std::cout << "Saved gray_cpu.png\n";
    std::cout << "CPU Grayscale time: " << cpu_time_ms << "ms\n";

    //GPU Grayscale
    Image gray_gpu = create_image(rgb.width, rgb.height, 1);
    //grayscale_gpu(rgb, gray_gpu);
    Timer gpu_timer;

    gpu_timer.start();

    grayscale_gpu(rgb, gray_gpu);

    double gpu_time_ms = gpu_timer.stop_ms();

    //convert to ocv for saving
    cv::Mat output_gpu = cvmat_from_image(gray_gpu);
    cv::imwrite("data/result_images/gray_gpu.png", output_gpu);

    std::cout << "Saved gray_gpu.png\n";
    std::cout << "GPU Grayscale time: " << gpu_time_ms << "ms\n";
    //mismatch check
    int mismatch = 0;
    for (int i = 0; i < rgb.width; ++i){
        if(gray.data[i] != gray_gpu.data[i]){
            mismatch++;
            if(mismatch < 10){
                std::cout << "Mismatch at " << i << ": CPU = " << (int)gray.data[i] << " GPU = " << (int)gray_gpu.data[i] << "\n";
            }
        }
    }
    std::cout << "Total mismatched pixels: " << mismatch << "\n";

    free_image(rgb);
    free_image(gray);
    free_image(gray_gpu);
    return 0;

}