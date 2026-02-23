#include <iostream>
#include <opencv2/opencv.hpp>
#include "image_utils.hpp"
#include "timer.hpp"
#include "gpu_context.hpp"
#include "grayscale_gpu.hpp"
#include "gaussian_gpu.hpp"

int main(){

    cv::Mat input = cv::imread("data/test_images/test_8k.jpg");

    if (input.empty()){
        std::cout << "Failed to load image\n OHHHH SHIAATTTT";
        return 1;
    }
    //convert OCV image to my struct Image
    Image rgb = image_from_cvmat(input);
    std::cout << "Loaded image: " << rgb.width << "x" << rgb.height << " channels = " << rgb.channels << "\n";

    std::cout << "Choose operation:\n";
    std::cout << "1. Grayscale\n";
    std::cout << "2. Gaussian Blur\n";
    int choice;
    std::cin >> choice;

    if (choice == 1) {
    // existing grayscale pipeline
        GrayscaleGPUContext gpu_ctx;
        grayscale_gpu_init(gpu_ctx, rgb.width, rgb.height);

        Image gray = create_image(rgb.width, rgb.height, 1); //allocate output 
        
        Timer cpu_timer;

        cpu_timer.start();
        //CPU Grayscale
        grayscale_cpu(rgb, gray);

        double cpu_time_ms = cpu_timer.stop_ms();

        //convert back to ocv for saving (imwrite)
        cv::Mat output = cvmat_from_image(gray);
        cv::imwrite("data/result_images/gray_cpu.png", output);

        std::cout << "Saved gray_cpu.png\n";
        std::cout << "CPU Grayscale time: " << cpu_time_ms << "ms\n";

        //GPU Grayscale
        Image gray_gpu = create_image(rgb.width, rgb.height, 1);
        //grayscale_gpu(rgb, gray_gpu);
        Timer gpu_timer;

        gpu_timer.start();

        grayscale_gpu_run(gpu_ctx, rgb, gray_gpu);

        double gpu_time_ms = gpu_timer.stop_ms();

        //convert to ocv for saving
        cv::Mat output_gpu = cvmat_from_image(gray_gpu);
        cv::imwrite("data/result_images/gray_gpu.png", output_gpu);

        std::cout << "Saved gray_gpu.png\n";
        std::cout << "GPU Grayscale time: " << gpu_time_ms << "ms\n";
        //mismatch check
        int mismatch = 0;
        for (int i = 0; i < rgb.width * rgb.height; ++i){
            if(gray.data[i] != gray_gpu.data[i]){
                mismatch++;
                if(mismatch < 10){
                    std::cout << "Mismatch at " << i << ": CPU = " << (int)gray.data[i] << " GPU = " << (int)gray_gpu.data[i] << "\n";
                }
            }
        }
        std::cout << "Total mismatched pixels: " << mismatch << "\n";
        grayscale_gpu_destroy(gpu_ctx);
        free_image(gray_gpu);
        free_image(gray);
    }   
    else if (choice == 2) {
        // Step 1: RGB → Grayscale (CPU)
        Image gray = create_image(rgb.width, rgb.height, 1);
        grayscale_cpu(rgb, gray);

        // Step 2: Allocate blurred output
        Image blurred = create_image(rgb.width, rgb.height, 1);
        Image temp1 = create_image(rgb.width, rgb.height, 1);
        Image temp2 = create_image(rgb.width, rgb.height, 1);

        // Step 3: Initialize Gaussian GPU
        GaussianGPUContext gauss_ctx;
        gaussian_gpu_init(gauss_ctx, gray.width, gray.height);

        Timer gpu_timer;
        gpu_timer.start();

        gaussian_gpu_run(gauss_ctx, gray.data, temp1.data);
        gaussian_gpu_run(gauss_ctx, temp1.data, temp2.data);
        gaussian_gpu_run(gauss_ctx, temp2.data, blurred.data);
        gaussian_gpu_run(gauss_ctx, blurred.data, temp1.data);
        gaussian_gpu_run(gauss_ctx, temp1.data, temp2.data);
        gaussian_gpu_run(gauss_ctx, temp2.data, blurred.data);

        double gpu_time_ms = gpu_timer.stop_ms();

        std::cout << "GPU Gaussian time: " << gpu_time_ms << "ms\n";

        // Step 4: Save result
        cv::Mat output_blur = cvmat_from_image(blurred);
        cv::imwrite("data/result_images/gaussian_gpu.png", output_blur);

        std::cout << "Saved gaussian_gpu.png\n";

        // Step 5: Cleanup
        gaussian_gpu_destroy(gauss_ctx);
        free_image(gray);
        free_image(blurred);
    // gaussian pipeline
    }
    else {
        std::cout << "Invalid option.\n";
    }

    //cleanup
    free_image(rgb);
    return 0;

}