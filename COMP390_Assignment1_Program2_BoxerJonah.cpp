/**
 * program 2: 2D graphics transformation
 * 
 * makes 5 houses aranged in a circle using rotations & translations
 */

// glut header
#include "GL/glut.h"
#include <math.h>

// Define M_PI if it isn't already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// draws one house
void drawHouse() {
    // draw the box (base of house)
    glBegin(GL_LINE_LOOP);
        glVertex2f(-0.1f, -0.1f);
        glVertex2f( 0.1f, -0.1f);
        glVertex2f( 0.1f,  0.1f);
        glVertex2f(-0.1f,  0.1f);
    glEnd();

    // draw roof (triangle)
    glBegin(GL_LINE_LOOP);
        glVertex2f(-0.1f, 0.1f);
        glVertex2f( 0.1f, 0.1f);
        glVertex2f( 0.0f, 0.2f);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f); // black lines
    
    // how many houses
    const int numHouses = 5;
    
    // how far from center
    const float radius = 0.6f;
    
    // draw all houses in circle
    for (int i = 0; i < numHouses; i++) {
        // calc angle for this house
        float angle = 2.0f * M_PI * i / numHouses;
        
        // save matrix
        glPushMatrix();
        
        // move to position on circle
        glTranslatef(radius * cos(angle), radius * sin(angle), 0.0f);
        
        // rotate house to face out from center
        // add 90 degs cuz house faces up by default
        glRotatef((angle * 180.0f / M_PI) + 90.0f, 0.0f, 0.0f, 1.0f);
        
        // draw the house
        drawHouse();
        
        // restore matrix
        glPopMatrix();
    }
    
    glFlush();
}

void init() {
    // set white background
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