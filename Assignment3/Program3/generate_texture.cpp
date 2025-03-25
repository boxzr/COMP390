/**
 * Texture Generator for Assignment 3: Program 3
 * 
 * This program generates a grass.bmp file with a checkerboard pattern
 * to be used as a texture in the OpenGL program.
 */

#include <stdio.h>
#include <stdlib.h>

// BMP file header structure
#pragma pack(push, 1)
typedef struct {
    unsigned short bfType;
    unsigned int   bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int   bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    unsigned int   biSize;
    int            biWidth;
    int            biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int   biCompression;
    unsigned int   biSizeImage;
    int            biXPelsPerMeter;
    int            biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

int main() {
    printf("Generating grass.bmp texture file...\n");
    
    // Dimensions (power of 2 for OpenGL texture compatibility)
    const int width = 256;
    const int height = 256;
    const int dataSize = width * height * 3;  // 3 bytes per pixel for RGB
    const int fileSize = 54 + dataSize;       // Header size + data size
    
    // Allocate memory for pixel data
    unsigned char* img = (unsigned char*)malloc(dataSize);
    if (!img) {
        printf("Error: Memory allocation failed\n");
        return 1;
    }
    
    // Create a checkerboard pattern
    printf("Creating checkerboard pattern...\n");
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Index in the data array (3 bytes per pixel)
            int idx = (y * width + x) * 3;
            
            // Create checkerboard pattern
            int isEvenX = (x / 32) % 2;
            int isEvenY = (y / 32) % 2;
            
            if ((isEvenX && isEvenY) || (!isEvenX && !isEvenY)) {
                // Dark green
                img[idx + 0] = 34;    // B
                img[idx + 1] = 139;   // G
                img[idx + 2] = 34;    // R
            } else {
                // Light green
                img[idx + 0] = 0;     // B
                img[idx + 1] = 252;   // G
                img[idx + 2] = 124;   // R
            }
        }
    }
    
    // Create and open the BMP file
    FILE* f = fopen("grass.bmp", "wb");
    if (!f) {
        printf("Error: Could not create output file\n");
        free(img);
        return 1;
    }
    
    // Initialize BMP header structures
    BITMAPFILEHEADER bfh = {0};
    bfh.bfType = 0x4D42;       // 'BM' in little endian
    bfh.bfSize = fileSize;
    bfh.bfOffBits = 54;        // Offset to image data (size of both headers)
    
    BITMAPINFOHEADER bih = {0};
    bih.biSize = 40;           // Size of BITMAPINFOHEADER
    bih.biWidth = width;
    bih.biHeight = height;
    bih.biPlanes = 1;
    bih.biBitCount = 24;       // 24 bits per pixel (3 bytes)
    
    // Write the file header
    printf("Writing BMP headers...\n");
    fwrite(&bfh.bfType, 2, 1, f);
    fwrite(&bfh.bfSize, 4, 1, f);
    fwrite(&bfh.bfReserved1, 2, 1, f);
    fwrite(&bfh.bfReserved2, 2, 1, f);
    fwrite(&bfh.bfOffBits, 4, 1, f);
    
    // Write the info header
    fwrite(&bih, 40, 1, f);
    
    // Write the image data
    printf("Writing pixel data...\n");
    fwrite(img, dataSize, 1, f);
    
    // Clean up
    fclose(f);
    free(img);
    
    printf("Successfully created grass.bmp (%dx%d)\n", width, height);
    return 0;
} 