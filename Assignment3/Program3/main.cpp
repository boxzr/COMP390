/**
 * Assignment 3: Program 3 - Loaded texture
 * 
 * This program extends Program 2 from Assignment 2 by adding texture
 * mapping to the scene. The program creates a scene with houses and
 * applies a texture to the ground.
 */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Display list ids
GLuint houseList;
GLuint groundList;

// Rotation angles for animation
float rotationAngle = 0.0f;

// Texture IDs
GLuint groundTexture;

// Loaded texture dimensions
int texWidth, texHeight;

// Raw texture data
unsigned char* textureData = NULL;

// Function to load a BMP texture
unsigned char* loadBMP(const char* filename, int* width, int* height) {
    FILE* file;
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    
    // Open the file
    file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Could not open texture file %s\n", filename);
        return NULL;
    }
    
    // Read the header
    if (fread(header, 1, 54, file) != 54) {
        printf("Error: Not a valid BMP file %s\n", filename);
        fclose(file);
        return NULL;
    }
    
    // Check the file is a BMP
    if (header[0] != 'B' || header[1] != 'M') {
        printf("Error: Not a valid BMP file %s\n", filename);
        fclose(file);
        return NULL;
    }
    
    // Read file info
    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    *width = *(int*)&(header[0x12]);
    *height = *(int*)&(header[0x16]);
    
    // If the size is missing, calculate it
    if (imageSize == 0) imageSize = (*width) * (*height) * 3;
    if (dataPos == 0) dataPos = 54; // BMP header is 54 bytes
    
    // Allocate memory for the texture
    unsigned char* data = (unsigned char*)malloc(imageSize);
    
    // Read the texture data
    fseek(file, dataPos, SEEK_SET);
    fread(data, 1, imageSize, file);
    
    // Close the file
    fclose(file);
    
    // Swap R and B (BMP is stored as BGR, OpenGL wants RGB)
    for (int i = 0; i < imageSize; i += 3) {
        unsigned char tmp = data[i];
        data[i] = data[i+2];
        data[i+2] = tmp;
    }
    
    return data;
}

// Load textures
void loadTextures() {
    // Load ground texture
    textureData = loadBMP("grass.bmp", &texWidth, &texHeight);
    
    if (!textureData) {
        printf("Failed to load texture. Using a procedural texture instead.\n");
        
        // Create a simple procedural texture if loading fails
        texWidth = 256;
        texHeight = 256;
        textureData = (unsigned char*)malloc(3 * texWidth * texHeight);
        
        for (int y = 0; y < texHeight; y++) {
            for (int x = 0; x < texWidth; x++) {
                // Checkerboard pattern
                int isEvenX = (x / 32) % 2;
                int isEvenY = (y / 32) % 2;
                
                if ((isEvenX && isEvenY) || (!isEvenX && !isEvenY)) {
                    // Darker green
                    textureData[3 * (y * texWidth + x) + 0] = 34;  // R
                    textureData[3 * (y * texWidth + x) + 1] = 139; // G
                    textureData[3 * (y * texWidth + x) + 2] = 34;  // B
                } else {
                    // Lighter green
                    textureData[3 * (y * texWidth + x) + 0] = 124; // R
                    textureData[3 * (y * texWidth + x) + 1] = 252; // G
                    textureData[3 * (y * texWidth + x) + 2] = 0;   // B
                }
            }
        }
    }
    
    // Generate texture
    glGenTextures(1, &groundTexture);
    
    // Bind and set texture parameters
    glBindTexture(GL_TEXTURE_2D, groundTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // Build mipmaps and upload texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texWidth, texHeight, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    
    // Free the texture data as it's now in GPU memory
    free(textureData);
    textureData = NULL;
}

// Create a display list for the basic house model
void createHouseList() {
    // Start a new display list
    houseList = glGenLists(1);
    glNewList(houseList, GL_COMPILE);
    
    // Draw ground (green)
    glColor3f(0.0f, 0.8f, 0.0f);
    glBegin(GL_QUADS);
        glVertex3f(-1.0f, 0.0f, -0.5f);  // far left
        glVertex3f(2.0f, 0.0f, -0.5f);   // far right
        glVertex3f(2.0f, 0.0f, 2.5f);    // near right
        glVertex3f(-1.0f, 0.0f, 2.5f);   // near left
    glEnd();
    
    // Draw cube body (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    
    // front face
    glBegin(GL_QUADS);
        glVertex3f(0.0f, 0.0f, 1.0f);  // bottom left
        glVertex3f(1.0f, 0.0f, 1.0f);  // bottom right
        glVertex3f(1.0f, 1.0f, 1.0f);  // top right
        glVertex3f(0.0f, 1.0f, 1.0f);  // top left
    glEnd();
    
    // back face
    glBegin(GL_QUADS);
        glVertex3f(0.0f, 0.0f, 0.0f);  // bottom left
        glVertex3f(1.0f, 0.0f, 0.0f);  // bottom right
        glVertex3f(1.0f, 1.0f, 0.0f);  // top right
        glVertex3f(0.0f, 1.0f, 0.0f);  // top left
    glEnd();
    
    // left face
    glBegin(GL_QUADS);
        glVertex3f(0.0f, 0.0f, 0.0f);  // bottom back
        glVertex3f(0.0f, 0.0f, 1.0f);  // bottom front
        glVertex3f(0.0f, 1.0f, 1.0f);  // top front
        glVertex3f(0.0f, 1.0f, 0.0f);  // top back
    glEnd();
    
    // right face
    glBegin(GL_QUADS);
        glVertex3f(1.0f, 0.0f, 0.0f);  // bottom back
        glVertex3f(1.0f, 0.0f, 1.0f);  // bottom front
        glVertex3f(1.0f, 1.0f, 1.0f);  // top front
        glVertex3f(1.0f, 1.0f, 0.0f);  // top back
    glEnd();
    
    // top face
    glBegin(GL_QUADS);
        glVertex3f(0.0f, 1.0f, 0.0f);  // left back
        glVertex3f(1.0f, 1.0f, 0.0f);  // right back
        glVertex3f(1.0f, 1.0f, 1.0f);  // right front
        glVertex3f(0.0f, 1.0f, 1.0f);  // left front
    glEnd();
    
    // bottom face
    glBegin(GL_QUADS);
        glVertex3f(0.0f, 0.0f, 0.0f);  // left back
        glVertex3f(1.0f, 0.0f, 0.0f);  // right back
        glVertex3f(1.0f, 0.0f, 1.0f);  // right front
        glVertex3f(0.0f, 0.0f, 1.0f);  // left front
    glEnd();
    
    // Draw pyramid top (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    
    // front face of pyramid
    glBegin(GL_TRIANGLES);
        glVertex3f(-0.2f, 1.0f, 1.2f);  // bottom left
        glVertex3f(1.2f, 1.0f, 1.2f);   // bottom right
        glVertex3f(0.5f, 1.5f, 0.5f);   // top
    glEnd();
    
    // right face of pyramid
    glBegin(GL_TRIANGLES);
        glVertex3f(1.2f, 1.0f, 1.2f);   // bottom front
        glVertex3f(1.2f, 1.0f, -0.2f);  // bottom back
        glVertex3f(0.5f, 1.5f, 0.5f);   // top
    glEnd();
    
    // back face of pyramid
    glBegin(GL_TRIANGLES);
        glVertex3f(1.2f, 1.0f, -0.2f);  // bottom right
        glVertex3f(-0.2f, 1.0f, -0.2f); // bottom left
        glVertex3f(0.5f, 1.5f, 0.5f);   // top
    glEnd();
    
    // left face of pyramid
    glBegin(GL_TRIANGLES);
        glVertex3f(-0.2f, 1.0f, -0.2f); // bottom back
        glVertex3f(-0.2f, 1.0f, 1.2f);  // bottom front
        glVertex3f(0.5f, 1.5f, 0.5f);   // top
    glEnd();
    
    glEndList();
}

// Create a display list for the textured ground
void createGroundList() {
    // Start a new display list
    groundList = glGenLists(1);
    glNewList(groundList, GL_COMPILE);
    
    // Draw textured ground
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, groundTexture);
    
    glColor3f(1.0f, 1.0f, 1.0f); // White to allow texture to show full color
    glBegin(GL_QUADS);
        // Repeat the texture several times across the ground
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-20.0f, -0.1f, -20.0f);  // far left
        
        glTexCoord2f(10.0f, 0.0f);
        glVertex3f(20.0f, -0.1f, -20.0f);   // far right
        
        glTexCoord2f(10.0f, 10.0f);
        glVertex3f(20.0f, -0.1f, 20.0f);    // near right
        
        glTexCoord2f(0.0f, 10.0f);
        glVertex3f(-20.0f, -0.1f, 20.0f);   // near left
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    glEndList();
}

// Draw a house with transformations
void drawHouse(float x, float z, float scale, float rotY) {
    glPushMatrix();
    
    // Apply transformations in order:
    // 1. translate to position
    glTranslatef(x, 0.0f, z);
    
    // 2. rotate around y-axis
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    
    // 3. scale the house
    glScalef(scale, scale, scale);
    
    // Draw the house using the display list
    glCallList(houseList);
    
    glPopMatrix();
}

// Draw the entire scene with multiple houses
void drawScene() {
    // Draw the textured ground first
    glCallList(groundList);
    
    // Draw houses in a circle
    const int numHouses = 8;
    const float radius = 12.0f;
    
    for (int i = 0; i < numHouses; i++) {
        // Calculate position on circle
        float angle = (float)i * (2.0f * 3.14159f / numHouses);
        float x = radius * cosf(angle);
        float z = radius * sinf(angle);
        
        // Calculate scaling (vary sizes a bit)
        float scale = 0.7f + 0.3f * sinf(angle * 2.0f);
        
        // Calculate rotation (face outward + some variation)
        float rotY = (angle * 180.0f / 3.14159f) + 180.0f;
        
        // Draw the house
        drawHouse(x, z, scale, rotY);
    }
    
    // Draw a house in the center
    drawHouse(0.0f, 0.0f, 1.0f, rotationAngle);
}

void display() {
    // Clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up view
    glLoadIdentity();
    
    // Position camera (eye, center, up)
    gluLookAt(20.0f, 15.0f, 20.0f,   // Eye position
              0.0f, 0.0f, 0.0f,     // Look at position
              0.0f, 1.0f, 0.0f);    // Up vector
    
    // Draw the scene
    drawScene();
    
    // Swap buffers
    glutSwapBuffers();
}

void reshape(int w, int h) {
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

void keyboard(unsigned char key, int x, int y) {
    // Exit on escape key
    if (key == 27) { // ESC
        exit(0);
    }
    
    // Toggle between solid and wireframe
    if (key == 'w' || key == 'W') {
        static bool wireframe = false;
        wireframe = !wireframe;
        if (wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        glutPostRedisplay();
    }
}

void timer(int value) {
    // Update rotation angle
    rotationAngle += 1.0f;
    if (rotationAngle > 360.0f) {
        rotationAngle -= 360.0f;
    }
    
    // Redraw scene
    glutPostRedisplay();
    
    // Reset timer for next frame
    glutTimerFunc(30, timer, 0);
}

void init() {
    // Set clear color to sky blue
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Enable color material
    glEnable(GL_COLOR_MATERIAL);
    
    // Load textures
    loadTextures();
    
    // Create display lists
    createHouseList();
    createGroundList();
    
    // Start animation timer
    glutTimerFunc(30, timer, 0);
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    
    // Use double buffering and depth testing
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    // Set window size and position
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    
    // Create window with title
    glutCreateWindow("Assignment 3 - Program 3: Textured Scene");
    
    // Init OpenGL state
    init();
    
    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    
    // Enter main loop
    glutMainLoop();
    
    return 0;
} 