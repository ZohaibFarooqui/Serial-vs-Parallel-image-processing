#include <iostream>
#include <cstdlib>
#include <time.h>
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

void applyBoxBlur(int width, int height, int channels, unsigned char *image, unsigned char *result) {
    for (int i = 1; i < height - 1; ++i) {
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
                     image[(i + 1) * width * channels + (j + 1) * channels + c]) / 9;
            }
        }
    }
}

int main() {
    clock_t start, end;
    double cpu_time_used;
    int width, height, channels;
    string image_path;

    cout << "Enter the image file path: ";
    cin >> image_path;

    start = clock();
    unsigned char *image = stbi_load(image_path.c_str(), &width, &height, &channels, 0);
    if (!image) {
        cerr << "Error loading image." << endl;
        return EXIT_FAILURE;
    }

    unsigned char *result = new unsigned char[width * height * channels];

    applyBoxBlur(width, height, channels, image, result);

    stbi_write_jpg("output.jpg", width, height, channels, result, 100);

    stbi_image_free(image);
    delete[] result;
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    cout << "Time taken to process the image serially: " << cpu_time_used * 1000 << " milliseconds" << endl;

    return 0;
}

