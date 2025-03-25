/**
 * program 3: coloring and shading
 * 
 * draws a square with colors at each corner to make
 * a color gradient
 */

// glut header
#include "GL/glut.h"

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // draw square with diff colors at corners
    glBegin(GL_POLYGON);
        // bottom-left: red
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-0.5f, -0.5f);
        
        // bottom-right: yellow
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(0.5f, -0.5f);
        
        // top-right: blue
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.5f, 0.5f);
        
        // top-left: green
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(-0.5f, 0.5f);
    glEnd();
    
    glFlush();
}

void init() {
    // white background
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    
    // smooth colors
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