#include <iostream>
#include <string>
#include <cstring> // For strcmp
#include <cstdlib> // For atoi
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_resize2.h"

using namespace std;

void printHelp(const char* programName) {
    cout << "Usage: " << programName << " --file-path <image_path> [--scale-factor <scale_factor>]" << endl;
    cout << "Options:" << endl;
    cout << "  --file-path    : Path to the image file (compulsory)" << endl;
    cout << "  --scale-factor : Factor by which to scale down the image (optional, default is 1)" << endl;
    cout << "  --help         : Show this help message" << endl;
}

int main(int argc, char* argv[]) {
    string filePath;
    int scaleFactor = 1; // Default value for scale factor

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            printHelp(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "--file-path") == 0 && i + 1 < argc) {
            filePath = argv[++i];
        } else if (strcmp(argv[i], "--scale-factor") == 0 && i + 1 < argc) {
            scaleFactor = atoi(argv[++i]);
        } else {
            cout << "Unknown option: " << argv[i] << endl;
            printHelp(argv[0]);
            return -1;
        }
    }

    if (filePath.empty()) {
        cout << "Error: --file-path is required." << endl;
        printHelp(argv[0]);
        return -1;
    }

    string Chars = " .:-=+*%#@";
    int CharsLen = Chars.length();
    
    int width, height, channels;
    unsigned char* ImageData = stbi_load(filePath.c_str(), &width, &height, &channels, 3);

    if (!ImageData) {
        cout << "Failed to load image" << endl;
        return -1;
    }

    // Define height correction factor to adjust for character aspect ratio
    const float heightCorrectionFactor = 2.0f; // Adjust this as necessary

    // Calculate new dimensions with height correction
    int newWidth = width / scaleFactor;
    int newHeight = static_cast<int>(height / (scaleFactor * heightCorrectionFactor));

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

            float avg = (R + G + B) / 3.0f;
            int charIndex = static_cast<int>(CharsLen * (avg / 255.0f));

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
