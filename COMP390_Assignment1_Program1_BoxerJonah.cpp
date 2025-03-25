/**
 * program 1: anti-aliasing
 * 
 * this program shows differences between regular lines and
 * anti-aliased ones when drawing 2 houses
 */

// include glut header with proper path
#include "GL/glut.h"

// draws a house with or without anti-aliasing
void drawHouse(float x, float y, float size, bool useAntiAliasing) {
    // setup anti-aliasing if needed
    if (useAntiAliasing) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glLineWidth(1.0f);
    } else {
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
        glLineWidth(1.0f);
    }

    // draw square (house base)
    glBegin(GL_LINE_LOOP);
        glVertex2f(x - size/2, y - size/2);
        glVertex2f(x + size/2, y - size/2);
        glVertex2f(x + size/2, y + size/2);
        glVertex2f(x - size/2, y + size/2);
    glEnd();

    // draw the roof (triangle)
    glBegin(GL_LINE_LOOP);
        glVertex2f(x - size/2, y + size/2);
        glVertex2f(x + size/2, y + size/2);
        glVertex2f(x, y + size);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f); // black lines
    
    // draw house without anti-aliasing (top)
    drawHouse(0.0f, 0.3f, 0.3f, false);
    
    // draw house with anti-aliasing (bottom)
    drawHouse(0.0f, -0.3f, 0.3f, true);
    
    glFlush();
}

void init() {
    // white background
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
    glutCreateWindow("Athabasca University - Comp390 TME 1 Program 1 Demo");
    
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
} 