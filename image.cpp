#include <iostream>
#include <cstdlib>
#include <chrono>
#include <omp.h>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

void applyBoxBlurParallel(int width, int height, int channels, unsigned char *image, unsigned char *result, double *threadTimes) {
    #pragma omp parallel
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        int chunk_size = height / num_threads;
        int start_row = thread_id * chunk_size;
        int end_row = (thread_id == num_threads - 1) ? height : (thread_id + 1) * chunk_size;

        int private_padding_row = 1;

        start_row = max(start_row - private_padding_row, 0);
        end_row = min(end_row + private_padding_row, height);

        double start_time = omp_get_wtime();

        for (int i = start_row; i < end_row; ++i) {
            for (int j = 1; j < width - 1; ++j) {
                for (int c = 0; c < channels; ++c) {
                    result[i * width * channels + j * channels + c] =
                        (image[(i - 1) * width * channels + (j - 1) * channels + c] +
                         image[(i - 1) * width * channels + j * channels + c] +
                         image[(i - 1) * width * channels + (j + 1) * channels + c] +
                         image[i * width * channels + (j - 1) * channels + c] +
                         image[i * width * channels + j * channels + c] +
                         image[i * width * channels + (j + 1) * channels + c] +
                         image[(i + 1) * width * channels + (j - 1) * channels + c] +
                         image[(i + 1) * width * channels + j * channels + c] +
                         image[(i + 1) * width * channels + (j + 1) * channels + c]) / 25;
                }
            }
        }

        double end_time = omp_get_wtime();
        double duration = end_time - start_time;

        #pragma omp critical
        {
            cout << "Thread " << thread_id << " took " << duration * 1000.0 << " milliseconds." << endl;
            threadTimes[thread_id] = duration;
        }
    }
}

int main(int argc, char *argv[]) {
    string image_path;
    cout << "Enter the image file path: ";
    cin >> image_path;
    /*if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <image_path>" << endl;
        return EXIT_FAILURE;
    }

    string image_path = argv[1];*/
    int width, height, channels;
    unsigned char *image = stbi_load(image_path.c_str(), &width, &height, &channels, 0);
    if (!image) {
        cerr << "Error loading image." << endl;
        return EXIT_FAILURE;
    }

    for (int num_processes = 4; num_processes <= 2048; num_processes *= 2) {
        omp_set_num_threads(num_processes);

        unsigned char *result = new unsigned char[width * height * channels];
        double *threadTimes = new double[num_processes];

        applyBoxBlurParallel(width, height, channels, image, result, threadTimes);
        stbi_write_jpg(("output_" + to_string(num_processes) + ".jpg").c_str(), width, height, channels, result, 100);

    
        double collectiveTime = 0.0;
        for (int i = 0; i < num_processes; ++i) {
            collectiveTime += threadTimes[i];
        }
        cout << "Execution time for " << num_processes << " threads: " << collectiveTime * 1000.0 << " milliseconds." << endl;

        delete[] result;
        delete[] threadTimes;
    }

    stbi_image_free(image);

    return 0;
}

