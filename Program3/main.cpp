/**
 * Program 3: Colouring and Shading
 * 
 * This program demonstrates coloring and shading in OpenGL.
 * It displays a square with four different colors at each vertex,
 * creating a smooth color gradient.
 */

#include <GL/glut.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw a square with different colors at each vertex
    glBegin(GL_POLYGON);
        // Bottom-left: Red
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-0.5f, -0.5f);
        
        // Bottom-right: Yellow
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(0.5f, -0.5f);
        
        // Top-right: Blue
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.5f, 0.5f);
        
        // Top-left: Green
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(-0.5f, 0.5f);
    glEnd();
    
    glFlush();
}

void init() {
    // Set the clear color to white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    
    // Enable smooth shading
    glShadeModel(GL_SMOOTH);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Athabasca University - Comp390 TME 1 Program 3 Demo");
    
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
} 