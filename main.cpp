#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_resize2.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <image_path>" << endl;
        return -1;
    }

    string Chars = " .:-=+*%#@";
    int CharsLen = Chars.length();
    
    int width, height, channels;
    unsigned char* ImageData = stbi_load(argv[1], &width, &height, &channels, 3);

    if (!ImageData) {
        cout << "Failed to load image" << endl;
        return -1;
    }

    // Define the downscale factor
    int scaleFactor = atoi(argv[2]); // Adjust this value to change the downscale factor

    // Calculate new dimensions
    int newWidth = width / scaleFactor;
    int newHeight = height / scaleFactor;

    // Allocate memory for resized image
    unsigned char* ResizedImageData = new unsigned char[newWidth * newHeight * 3];

    // Perform the resizing
    stbir_resize_uint8_linear(ImageData, width, height, 0, ResizedImageData, newWidth, newHeight, 0, (stbir_pixel_layout)3);

    // Iterate over each pixel in the resized image
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            int idx = (x + y * newWidth) * 3; // Index in resized image
            unsigned char R = ResizedImageData[idx];
            unsigned char G = ResizedImageData[idx + 1];
            unsigned char B = ResizedImageData[idx + 2];

            float avg = (R + G + B) / 3.0;
            int charIndex = CharsLen * (avg / 255.0);

            if (charIndex >= CharsLen) charIndex = CharsLen - 1;

            cout << Chars[charIndex];
        }
        cout << endl;
    }

    // Clean up
    delete[] ResizedImageData;
    stbi_image_free(ImageData);

    return 0;
}
