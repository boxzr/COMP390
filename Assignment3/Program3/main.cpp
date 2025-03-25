/**
 * Assignment 3: Program 3 - Texture Mapping
 * 
 * This program demonstrates texture mapping on a 3D object.
 * It includes loading a texture from a BMP file and applying it to a 3D model.
 */

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Fix for exit function conflict
#undef exit

// Define M_PI if not available
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Debug mode for error checking
#ifdef DEBUG
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT(...)
#endif

// Texture parameters
GLuint texture = 0;
const char* textureFilename = "grass.bmp";
bool texturingEnabled = true;

// Rotation angles
float xRotation = 0.0f;
float yRotation = 0.0f;

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

// Function to load a BMP file
unsigned char* loadBMP(const char* filename, int* width, int* height) {
    FILE* file;
    unsigned char* data;
    unsigned char* flipped_data;
    BITMAPFILEHEADER header;
    BITMAPINFOHEADER infoHeader;
    int dataSize;
    int padding;
    int i, j;
    
    DEBUG_PRINT("Loading BMP file: %s\n", filename);
    
    // Open file
    file = fopen(filename, "rb");
    if (!file) {
        DEBUG_PRINT("Error: Could not open %s\n", filename);
        return NULL;
    }
    
    // Read BMP header
    fread(&header.bfType, sizeof(unsigned short), 1, file);
    fread(&header.bfSize, sizeof(unsigned int), 1, file);
    fread(&header.bfReserved1, sizeof(unsigned short), 1, file);
    fread(&header.bfReserved2, sizeof(unsigned short), 1, file);
    fread(&header.bfOffBits, sizeof(unsigned int), 1, file);
    
    if (header.bfType != 0x4D42) { // 'BM' in little endian
        DEBUG_PRINT("Error: Not a valid BMP file (wrong magic number)\n");
        fclose(file);
        return NULL;
    }
    
    // Read BMP info header
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);
    
    if (infoHeader.biBitCount != 24) {
        DEBUG_PRINT("Error: Only 24-bit BMPs are supported\n");
        fclose(file);
        return NULL;
    }
    
    // Set width and height
    *width = infoHeader.biWidth;
    *height = infoHeader.biHeight;
    
    DEBUG_PRINT("BMP dimensions: %d x %d\n", *width, *height);
    
    // Calculate row padding for BMP (rows are aligned to 4-byte boundaries)
    padding = (4 - ((*width) * 3) % 4) % 4;
    
    // Calculate data size (including padding)
    dataSize = (*width) * (*height) * 3;
    
    // Allocate memory for data without padding
    data = (unsigned char*)malloc(dataSize);
    flipped_data = (unsigned char*)malloc(dataSize);
    
    if (!data || !flipped_data) {
        DEBUG_PRINT("Error: Memory allocation failed\n");
        fclose(file);
        free(data);
        free(flipped_data);
        return NULL;
    }
    
    // Seek to start of image data
    fseek(file, header.bfOffBits, SEEK_SET);
    
    // Read data row by row, skipping padding
    for (i = 0; i < *height; i++) {
        fread(&data[i * (*width) * 3], 3, *width, file);
        
        // Skip padding
        fseek(file, padding, SEEK_CUR);
    }
    
    // Flip the image and convert BGR to RGB
    for (i = 0; i < *height; i++) {
        for (j = 0; j < *width; j++) {
            int dst = (((*height) - 1 - i) * (*width) + j) * 3;
            int src = (i * (*width) + j) * 3;
            
            // Swap B and R (BGR -> RGB)
            flipped_data[dst] = data[src + 2];     // R
            flipped_data[dst + 1] = data[src + 1]; // G
            flipped_data[dst + 2] = data[src];     // B
        }
    }
    
    free(data);
    fclose(file);
    
    DEBUG_PRINT("BMP loaded successfully\n");
    return flipped_data;
}

// Load texture from BMP file
bool loadTexture() {
    int width, height;
    unsigned char* data;
    
    // Load BMP file
    data = loadBMP(textureFilename, &width, &height);
    
    if (!data) {
        DEBUG_PRINT("Failed to load texture\n");
        return false;
    }
    
    // Generate texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Set texture image data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    // Free texture data
    free(data);
    
    DEBUG_PRINT("Texture loaded successfully: ID=%d, size=%dx%d\n", texture, width, height);
    return true;
}

// Draw a textured cube
void drawTexturedCube() {
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glBegin(GL_QUADS);
    
    // Front face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    
    // Back face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    
    // Top face
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    
    // Bottom face
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    
    // Right face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    
    // Left face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    
    glEnd();
}

// Draw a textured sphere
void drawTexturedSphere(float radius, int slices, int stacks) {
    float drho = M_PI / stacks;
    float dtheta = 2.0f * M_PI / slices;
    float ds = 1.0f / slices;
    float dt = 1.0f / stacks;
    float t = 1.0f;
    float s;
    int i, j;
    
    glBindTexture(GL_TEXTURE_2D, texture);
    
    for (i = 0; i < stacks; i++) {
        float rho = i * drho;
        float srho = (float)sin(rho);
        float crho = (float)cos(rho);
        float srhodrho = (float)sin(rho + drho);
        float crhodrho = (float)cos(rho + drho);
        
        // Many small strips
        glBegin(GL_TRIANGLE_STRIP);
        s = 0.0f;
        
        for (j = 0; j <= slices; j++) {
            float theta = (j == slices) ? 0.0f : j * dtheta;
            float stheta = (float)-sin(theta);
            float ctheta = (float)cos(theta);
            
            // Top vertex
            glTexCoord2f(s, t - dt);
            glNormal3f(srhodrho * stheta, crhodrho, srhodrho * ctheta);
            glVertex3f(srhodrho * stheta * radius, crhodrho * radius, srhodrho * ctheta * radius);
            
            // Bottom vertex
            glTexCoord2f(s, t);
            glNormal3f(srho * stheta, crho, srho * ctheta);
            glVertex3f(srho * stheta * radius, crho * radius, srho * ctheta * radius);
            
            s += ds;
        }
        
        glEnd();
        t -= dt;
    }
}

// Display function
void display() {
    DEBUG_PRINT("Display function called\n");
    
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Reset transformations
    glLoadIdentity();
    
    // Set the camera position
    gluLookAt(0.0, 0.0, 5.0,  // Eye position
              0.0, 0.0, 0.0,  // Look-at position
              0.0, 1.0, 0.0); // Up vector
    
    // Apply rotations
    glRotatef(xRotation, 1.0f, 0.0f, 0.0f);
    glRotatef(yRotation, 0.0f, 1.0f, 0.0f);
    
    // Draw axes
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINES);
        // X axis (red)
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(3.0f, 0.0f, 0.0f);
        
        // Y axis (green)
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 3.0f, 0.0f);
        
        // Z axis (blue)
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 3.0f);
    glEnd();
    
    // Draw textured object
    if (texturingEnabled) {
        DEBUG_PRINT("Drawing with texture\n");
        glEnable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);  // White to show texture colors
    } else {
        DEBUG_PRINT("Drawing without texture\n");
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.7f, 0.7f, 0.7f);  // Grey for untextured
    }
    
    // Draw a textured cube and sphere
    glPushMatrix();
    glTranslatef(-1.5f, 0.0f, 0.0f);
    drawTexturedCube();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1.5f, 0.0f, 0.0f);
    drawTexturedSphere(1.0f, 30, 30);
    glPopMatrix();
    
    // Display information text
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Restore transformation matrix
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    // Display status in top left corner
    glRasterPos3f(-2.5, 2.0, 0.0);
    if (texturingEnabled) {
        const char* text = "Texturing Enabled (Press 'T' to toggle)";
        for (int i = 0; text[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
        }
    } else {
        const char* text = "Texturing Disabled (Press 'T' to toggle)";
        for (int i = 0; text[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
        }
    }
    
    glutSwapBuffers();
}

// Reshape function
void reshape(int w, int h) {
    DEBUG_PRINT("Reshape called: w=%d, h=%d\n", w, h);
    
    // Prevent division by zero
    if (h == 0) h = 1;
    
    // Set viewport to window dimensions
    glViewport(0, 0, w, h);
    
    // Set perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 0.1f, 100.0f);
    
    // Switch back to modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard callback
void keyboard(unsigned char key, int x, int y) {
    DEBUG_PRINT("Keyboard input: key=%c, x=%d, y=%d\n", key, x, y);
    
    // Exit on escape key
    if (key == 27) { // ESC
        DEBUG_PRINT("ESC pressed, exiting\n");
        glutDestroyWindow(glutGetWindow());
        return;
    }
    
    // Toggle texturing
    if (key == 't' || key == 'T') {
        texturingEnabled = !texturingEnabled;
        DEBUG_PRINT("Texturing toggled: %s\n", texturingEnabled ? "enabled" : "disabled");
        glutPostRedisplay();
    }
    
    // Rotate view
    if (key == 'w' || key == 'W') {
        xRotation -= 5.0f;
        glutPostRedisplay();
    }
    if (key == 's' || key == 'S') {
        xRotation += 5.0f;
        glutPostRedisplay();
    }
    if (key == 'a' || key == 'A') {
        yRotation -= 5.0f;
        glutPostRedisplay();
    }
    if (key == 'd' || key == 'D') {
        yRotation += 5.0f;
        glutPostRedisplay();
    }
}

// Special key callback
void specialKeys(int key, int x, int y) {
    DEBUG_PRINT("Special key input: key=%d, x=%d, y=%d\n", key, x, y);
    
    if (key == GLUT_KEY_UP) {
        xRotation -= 5.0f;
    }
    if (key == GLUT_KEY_DOWN) {
        xRotation += 5.0f;
    }
    if (key == GLUT_KEY_LEFT) {
        yRotation -= 5.0f;
    }
    if (key == GLUT_KEY_RIGHT) {
        yRotation += 5.0f;
    }
    
    glutPostRedisplay();
}

// Initialization function
void init() {
    DEBUG_PRINT("Initializing OpenGL settings\n");
    
    // Set background color to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Load texture
    if (!loadTexture()) {
        DEBUG_PRINT("Warning: Failed to load texture\n");
    }
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    
    DEBUG_PRINT("Starting Program 3: Texture Mapping\n");
    DEBUG_PRINT("GLUT initialized\n");
    
    // Use double buffering and depth testing
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    // Set window size and position
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    
    // Create window with title
    glutCreateWindow("Assignment 3 - Program 3: Texture Mapping");
    DEBUG_PRINT("Window created\n");
    
    // Initialize rendering
    init();
    
    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    DEBUG_PRINT("Entering main loop...\n");
    // Enter main loop
    glutMainLoop();
    
    return 0;
} 