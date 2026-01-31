#include <cuda_runtime.h>
#include <cstdint>

__global__ void gaussian_blur_3x3(
    const uint8_t* input,
    uint8_t* output,
    int width,
    int height
) {
    int tx = threadIdx.x;
    int ty = threadIdx.y;                       //thread indexes

    int gx = blockIdx.x * blockDim.x + tx;      
    int gy = blockIdx.y * blockDim.y + ty;      //global indices

    __shared__ uint8_t tile[18][18];
    int sx = tx + 1;
    int sy = ty + 1;                           // shared mem with halo

    auto mirror = [](int v, int limit) {
        if (v < 0) return -v;
        if (v >= limit) return 2 * limit - v - 2;       //mirror helper
        return v;
    };

    int mx = mirror(gx, width);
    int my = mirror(gy, height);

    tile[sy][sx] = input[my * width + mx];      //central pixel

    //left halo
    if (tx == 0){
        int hx = mirror(gx - 1, width);
        tile[sy][sx-1] = input[my * width + hx];
    }

    //right halo
    if (tx == blockDim.x-1){
        int hx = mirror(gx + 1, width);
        tile[sy][sx+1] = input[my * width + hx];
    }

    //top halo
    if(ty == 0){
        int hy = mirror(gy - 1, height);
        tile[sy-1][sx] = input [hy * width + mx];
    }

    //bottom halo
    if(ty == blockDim.y -1){
        int hy = mirror(gy +1, height);
        tile[sy+1][sx] = input[hy * width + mx];
    }

    //corner halos
    //topleft
    if(tx==0 && ty==0){
        tile[sy-1][sx-1] = input[mirror(gy - 1, height) * width + mirror(gx - 1, width)];
    }
    //topright
    if(tx==blockDim.x-1 && ty ==0){
        tile[sy-1][sx+1] = input[mirror(gy+1, height) * width + mirror(gx+1,width)];

    }
    //bottomleft
    if(tx==0 && ty==blockDim.y-1){
        tile[sy+1][sx-1] = input[mirror(gy+1,height) * width + mirror(gx-1,width)];
    }
    //bottomright
    if(tx==blockDim.x-1 && ty==blockDim.y-1){
        tile[sy+1][sy+1] = input[mirror(gy+1,height) * width + mirror(gx+1, width)];
    }
    
    __syncthreads();

    if(gx < width && gy < height) {
        int sum = 
            1 * tile[sy - 1][sx - 1] + 2 * tile[sy -1][sx] + 1 * tile[sy -1][sx + 1] +
            2 * tile[sy][sx - 1] + 4 * tile[sy][sx] + 2 * tile[sy][sx+1] +
            1 * tile[sy +1 ][sx - 1] + 2 * tile[sy + 1][sx] + 1 * tile[sy + 1][sx + 1];
            
        output[gy * width + gx] = static_cast<uint8_t>(sum / 16);
    }

}