/**
 * Program 2: 2D Graphics Transformation
 * 
 * This program demonstrates 2D transformations in OpenGL.
 * It displays five houses arranged in a circle using rotation and translation.
 */

#include <GL/glut.h>
#include <math.h>

// Function to draw a single house
void drawHouse() {
    // Draw the rectangle (house base)
    glBegin(GL_LINE_LOOP);
        glVertex2f(-0.1f, -0.1f);
        glVertex2f( 0.1f, -0.1f);
        glVertex2f( 0.1f,  0.1f);
        glVertex2f(-0.1f,  0.1f);
    glEnd();

    // Draw the triangle (roof)
    glBegin(GL_LINE_LOOP);
        glVertex2f(-0.1f, 0.1f);
        glVertex2f( 0.1f, 0.1f);
        glVertex2f( 0.0f, 0.2f);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f); // Black lines
    
    // Number of houses to draw in a circle
    const int numHouses = 5;
    
    // Radius of the circle
    const float radius = 0.6f;
    
    // Draw houses in a circle
    for (int i = 0; i < numHouses; i++) {
        // Calculate angle for this house (in radians)
        float angle = 2.0f * M_PI * i / numHouses;
        
        // Save the current transformation matrix
        glPushMatrix();
        
        // Translate to position on circle
        glTranslatef(radius * cos(angle), radius * sin(angle), 0.0f);
        
        // Rotate the house to face outward from center
        // We add 90 degrees (π/2) because our house faces up by default
        glRotatef((angle * 180.0f / M_PI) + 90.0f, 0.0f, 0.0f, 1.0f);
        
        // Draw the house at the transformed position
        drawHouse();
        
        // Restore the previous transformation matrix
        glPopMatrix();
    }
    
    glFlush();
}

void init() {
    // Set the clear color to white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Athabasca University - Comp390 TME 1 Program 2 Demo");
    
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
} 