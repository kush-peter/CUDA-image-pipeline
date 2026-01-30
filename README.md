FROM SCRATCH CUDA C++ IMAGE PROCESSING PIPELINE FOCUSED ON MEMORY LAYOUT, KERNEL DESIGN AND PERF BENCHMARKING

3-channel images = BGR
1-channel images = grayscale

//grayscale math
uint8_t b = input.data[idx + 0];
uint8_t g = input.data[idx + 1];
uint8_t r = input.data[idx + 2];

float gray = 0.299f*r + 0.587f*g + 0.114f*b;
