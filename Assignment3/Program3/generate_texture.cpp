#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// BMP file header structure
#pragma pack(push, 1)
typedef struct {
    unsigned char signature[2];  // 'BM'
    unsigned int fileSize;       // File size in bytes
    unsigned int reserved;       // Reserved
    unsigned int dataOffset;     // Offset to image data
    unsigned int headerSize;     // Header size
    int width;                   // Width of the image
    int height;                  // Height of the image
    unsigned short planes;       // Number of color planes
    unsigned short bitsPerPixel; // Bits per pixel
    unsigned int compression;    // Compression type
    unsigned int imageSize;      // Image size
    int xPixelsPerMeter;         // Pixels per meter in X
    int yPixelsPerMeter;         // Pixels per meter in Y
    unsigned int colorsUsed;     // Number of colors used
    unsigned int colorsImportant;// Important colors
} BMPHeader;
#pragma pack(pop)

// Simple fast random number generator
float randomFloat() {
    return (float)rand() / RAND_MAX;
}

// Generate grass texture
void generateGrassTexture(unsigned char* buffer, int width, int height) {
    // Base grass color (medium green)
    unsigned char baseR = 34;
    unsigned char baseG = 139;
    unsigned char baseB = 34;
    
    // Lighter grass color
    unsigned char lightR = 124;
    unsigned char lightG = 252;
    unsigned char lightB = 0;
    
    // Fill texture with base color
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Index in the buffer (BGR order for BMP)
            int idx = (y * width + x) * 3;
            
            // Start with base color
            buffer[idx] = baseB;     // B
            buffer[idx + 1] = baseG; // G
            buffer[idx + 2] = baseR; // R
            
            // Add some random variation
            float noise = randomFloat() * 0.3f + 0.7f;
            buffer[idx] = (unsigned char)(buffer[idx] * noise);
            buffer[idx + 1] = (unsigned char)(buffer[idx + 1] * noise);
            buffer[idx + 2] = (unsigned char)(buffer[idx + 2] * noise);
            
            // Add lighter blades of grass
            if (randomFloat() < 0.05f) {
                // Create vertical blade of grass
                int bladeHeight = 5 + rand() % 15; // Random height
                for (int i = 0; i < bladeHeight && (y + i) < height; i++) {
                    int bladeIdx = ((y + i) * width + x) * 3;
                    float fade = 1.0f - ((float)i / bladeHeight); // Fade towards top
                    
                    // Blend with lighter color
                    buffer[bladeIdx] = (unsigned char)(lightB * fade + buffer[bladeIdx] * (1 - fade));
                    buffer[bladeIdx + 1] = (unsigned char)(lightG * fade + buffer[bladeIdx + 1] * (1 - fade));
                    buffer[bladeIdx + 2] = (unsigned char)(lightR * fade + buffer[bladeIdx + 2] * (1 - fade));
                }
            }
            
            // Add darker patches (like shadows or dirt)
            if (randomFloat() < 0.02f) {
                int patchSize = 3 + rand() % 8;
                for (int dy = -patchSize; dy <= patchSize; dy++) {
                    for (int dx = -patchSize; dx <= patchSize; dx++) {
                        int nx = x + dx;
                        int ny = y + dy;
                        
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            float dist = sqrtf(dx*dx + dy*dy) / patchSize;
                            if (dist <= 1.0f) {
                                float darkness = 0.7f * (1.0f - dist);
                                int patchIdx = (ny * width + nx) * 3;
                                buffer[patchIdx] = (unsigned char)(buffer[patchIdx] * (1.0f - darkness));
                                buffer[patchIdx + 1] = (unsigned char)(buffer[patchIdx + 1] * (1.0f - darkness));
                                buffer[patchIdx + 2] = (unsigned char)(buffer[patchIdx + 2] * (1.0f - darkness));
                            }
                        }
                    }
                }
            }
        }
    }
}

int main() {
    // Texture dimensions (must be power of 2 for OpenGL)
    const int width = 256;
    const int height = 256;
    
    // Seed random number generator
    srand((unsigned int)time(NULL));
    
    // Allocate texture buffer (3 bytes per pixel for RGB)
    unsigned char* buffer = (unsigned char*)malloc(width * height * 3);
    if (!buffer) {
        printf("Failed to allocate memory for texture\n");
        return 1;
    }
    
    // Fill the buffer with grass texture
    generateGrassTexture(buffer, width, height);
    
    // Write texture to BMP file
    FILE* file = fopen("grass.bmp", "wb");
    if (!file) {
        printf("Failed to create the output file\n");
        free(buffer);
        return 1;
    }
    
    // Calculate file size
    int paddedWidth = (width * 3 + 3) & ~3; // Each row must be a multiple of 4 bytes
    int imageSize = paddedWidth * height;
    int fileSize = 54 + imageSize; // Header (54 bytes) + image data
    
    // Create and write BMP header
    BMPHeader header = {0};
    header.signature[0] = 'B';
    header.signature[1] = 'M';
    header.fileSize = fileSize;
    header.reserved = 0;
    header.dataOffset = 54;
    header.headerSize = 40;
    header.width = width;
    header.height = height;
    header.planes = 1;
    header.bitsPerPixel = 24;
    header.compression = 0;
    header.imageSize = imageSize;
    header.xPixelsPerMeter = 0;
    header.yPixelsPerMeter = 0;
    header.colorsUsed = 0;
    header.colorsImportant = 0;
    
    fwrite(&header, sizeof(BMPHeader), 1, file);
    
    // Write image data (BMP format requires padding at the end of each row)
    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = paddedWidth - (width * 3);
    
    // BMPs are stored bottom-to-top
    for (int y = height - 1; y >= 0; y--) {
        // Write the row data
        fwrite(&buffer[y * width * 3], 3, width, file);
        
        // Write padding if needed
        if (paddingSize > 0) {
            fwrite(padding, 1, paddingSize, file);
        }
    }
    
    // Close the file and free memory
    fclose(file);
    free(buffer);
    
    printf("Grass texture created successfully: grass.bmp\n");
    return 0;
} 