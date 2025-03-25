/**
 * Assignment 3: Program 2 - Calculate specular contribution
 * 
 * This program demonstrates the calculation of lighting intensity
 * including specular contribution. The viewer is positioned at a 
 * distance of at least 100 units to visualize the specular highlights.
 */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

// Material properties
GLfloat mat_ambient[] = {0.24725f, 0.1995f, 0.0745f, 1.0f};
GLfloat mat_diffuse[] = {0.75164f, 0.60648f, 0.22658f, 1.0f};
GLfloat mat_specular[] = {0.628281f, 0.555802f, 0.366065f, 1.0f};
GLfloat mat_shininess[] = {51.2f};  // Lower shininess for more visible highlights

// Light position and properties
GLfloat light_position[] = {2.0f, 2.0f, 2.0f, 1.0f};
GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

// Viewer position (z >= 100)
GLfloat viewer_position[] = {0.0f, 0.0f, 100.0f};

// Rotation angles
float xRotation = 0.0f;
float yRotation = 0.0f;

// Flag to show calculated lighting
bool useCalculatedLighting = false;

// Structure to represent a point in 3D space
typedef struct {
    GLfloat x, y, z;
} Point3D;

// Structure to represent a vector in 3D space
typedef struct {
    GLfloat x, y, z;
} Vector3D;

// Structure to represent a color
typedef struct {
    GLfloat r, g, b;
} Color;

// Normalize a vector
Vector3D normalize(Vector3D v) {
    Vector3D result;
    float length = (float)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length > 0.0001f) {
        result.x = v.x / length;
        result.y = v.y / length;
        result.z = v.z / length;
    } else {
        result.x = result.y = result.z = 0.0f;
    }
    return result;
}

// Calculate dot product of two vectors
float dotProduct(Vector3D v1, Vector3D v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Calculate vector from two points
Vector3D calculateVector(Point3D from, Point3D to) {
    Vector3D result;
    result.x = to.x - from.x;
    result.y = to.y - from.y;
    result.z = to.z - from.z;
    return result;
}

// Calculate light intensity at a point
Color calculateLightingAtPoint(Point3D point, Vector3D normal) {
    Color result = {0.0f, 0.0f, 0.0f};
    
    // Convert light position to Point3D
    Point3D lightPoint = {light_position[0], light_position[1], light_position[2]};
    
    // Calculate light vector (from point to light)
    Vector3D lightVector = calculateVector(point, lightPoint);
    lightVector = normalize(lightVector);
    
    // Calculate view vector (from point to viewer)
    Point3D viewerPoint = {viewer_position[0], viewer_position[1], viewer_position[2]};
    Vector3D viewVector = calculateVector(point, viewerPoint);
    viewVector = normalize(viewVector);
    
    // Ambient component
    result.r += mat_ambient[0] * light_ambient[0];
    result.g += mat_ambient[1] * light_ambient[1];
    result.b += mat_ambient[2] * light_ambient[2];
    
    // Diffuse component (N·L)
    float diffuseFactor = dotProduct(normal, lightVector);
    if (diffuseFactor > 0.0f) {
        result.r += diffuseFactor * mat_diffuse[0] * light_diffuse[0];
        result.g += diffuseFactor * mat_diffuse[1] * light_diffuse[1];
        result.b += diffuseFactor * mat_diffuse[2] * light_diffuse[2];
        
        // Specular component (R·V)^shininess
        // Calculate reflection vector
        Vector3D reflectVector;
        reflectVector.x = 2.0f * diffuseFactor * normal.x - lightVector.x;
        reflectVector.y = 2.0f * diffuseFactor * normal.y - lightVector.y;
        reflectVector.z = 2.0f * diffuseFactor * normal.z - lightVector.z;
        reflectVector = normalize(reflectVector);
        
        float specularFactor = dotProduct(reflectVector, viewVector);
        if (specularFactor > 0.0f) {
            specularFactor = (float)pow(specularFactor, mat_shininess[0]);
            result.r += specularFactor * mat_specular[0] * light_specular[0];
            result.g += specularFactor * mat_specular[1] * light_specular[1];
            result.b += specularFactor * mat_specular[2] * light_specular[2];
        }
    }
    
    // Clamp values to valid range
    if (result.r > 1.0f) result.r = 1.0f;
    if (result.g > 1.0f) result.g = 1.0f;
    if (result.b > 1.0f) result.b = 1.0f;
    
    return result;
}

// Draw a sphere with custom lighting calculations
void drawCustomLitSphere(float radius, int slices, int stacks) {
    DEBUG_PRINT("Drawing custom lit sphere with: radius=%f, slices=%d, stacks=%d\n", 
               radius, slices, stacks);
               
    for (int i = 0; i < stacks; ++i) {
        float phi1 = (float)(M_PI * i / stacks);
        float phi2 = (float)(M_PI * (i + 1) / stacks);
        
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float theta = (float)(2.0f * M_PI * j / slices);
            
            // Calculate points on the sphere
            Point3D p1 = {
                radius * (float)sin(phi1) * (float)cos(theta),
                radius * (float)cos(phi1),
                radius * (float)sin(phi1) * (float)sin(theta)
            };
            
            Point3D p2 = {
                radius * (float)sin(phi2) * (float)cos(theta),
                radius * (float)cos(phi2),
                radius * (float)sin(phi2) * (float)sin(theta)
            };
            
            // Calculate normals (normalized position vector)
            Vector3D n1 = {p1.x / radius, p1.y / radius, p1.z / radius};
            Vector3D n2 = {p2.x / radius, p2.y / radius, p2.z / radius};
            
            // Calculate lighting at the points
            Color c1 = calculateLightingAtPoint(p1, n1);
            Color c2 = calculateLightingAtPoint(p2, n2);
            
            // Set color and draw vertices
            glColor3f(c1.r, c1.g, c1.b);
            glVertex3f(p1.x, p1.y, p1.z);
            
            glColor3f(c2.r, c2.g, c2.b);
            glVertex3f(p2.x, p2.y, p2.z);
        }
        glEnd();
    }
}

// Display function
void display() {
    DEBUG_PRINT("Display function called\n");
    
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Reset transformations
    glLoadIdentity();
    
    // Set the camera position (far away for proper specular highlights)
    gluLookAt(viewer_position[0], viewer_position[1], viewer_position[2],  // Eye position
              0.0, 0.0, 0.0,                             // Look-at position
              0.0, 1.0, 0.0);                           // Up vector
    
    // Apply rotations
    glRotatef(xRotation, 1.0f, 0.0f, 0.0f);
    glRotatef(yRotation, 0.0f, 1.0f, 0.0f);
    
    // Set light position (will be transformed by the modelview matrix)
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    // Draw axes
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
        // X axis (red)
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(5.0f, 0.0f, 0.0f);
        
        // Y axis (green)
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 5.0f, 0.0f);
        
        // Z axis (blue)
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 5.0f);
    glEnd();
    
    // Draw light source (small yellow sphere)
    glPushMatrix();
    glTranslatef(light_position[0], light_position[1], light_position[2]);
    glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.2, 10, 10);
    glPopMatrix();
    
    // Draw sphere
    if (useCalculatedLighting) {
        // Draw using custom lighting calculations
        DEBUG_PRINT("Using custom lighting calculations\n");
        glDisable(GL_LIGHTING);
        drawCustomLitSphere(2.0, 30, 30);
    } else {
        // Draw using OpenGL's built-in lighting
        DEBUG_PRINT("Using OpenGL built-in lighting\n");
        glEnable(GL_LIGHTING);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glutSolidSphere(2.0, 30, 30);
    }
    
    // Display information text
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Restore transformation matrix
    glLoadIdentity();
    gluLookAt(viewer_position[0], viewer_position[1], viewer_position[2], 
              0.0, 0.0, 0.0, 
              0.0, 1.0, 0.0);
    
    // Display status in top left corner
    glRasterPos3f(-9.0, 8.0, -20.0);
    if (useCalculatedLighting) {
        const char* text = "Using custom lighting calculations";
        for (int i = 0; text[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
        }
    } else {
        const char* text = "Using OpenGL built-in lighting";
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
    gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 1.0f, 1000.0f);
    
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
    
    // Toggle between OpenGL lighting and custom lighting calculations
    if (key == 'l' || key == 'L') {
        useCalculatedLighting = !useCalculatedLighting;
        DEBUG_PRINT("Lighting mode toggled: useCalculatedLighting=%d\n", useCalculatedLighting);
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
    
    // Move light
    if (key == '6') {
        light_position[0] += 0.5f;
        glutPostRedisplay();
    }
    if (key == '4') {
        light_position[0] -= 0.5f;
        glutPostRedisplay();
    }
    if (key == '8') {
        light_position[1] += 0.5f;
        glutPostRedisplay();
    }
    if (key == '2') {
        light_position[1] -= 0.5f;
        glutPostRedisplay();
    }
    if (key == '9') {
        light_position[2] += 0.5f;
        glutPostRedisplay();
    }
    if (key == '1') {
        light_position[2] -= 0.5f;
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
    
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Set light properties
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    // Set smooth shading
    glShadeModel(GL_SMOOTH);
    
    // Enable normalization of normals for proper lighting
    glEnable(GL_NORMALIZE);
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    
    DEBUG_PRINT("Starting Program 2: Specular Lighting Calculation\n");
    DEBUG_PRINT("GLUT initialized\n");
    
    // Use double buffering and depth testing
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    // Set window size and position
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    
    // Create window with title
    glutCreateWindow("Assignment 3 - Program 2: Specular Lighting Calculation");
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