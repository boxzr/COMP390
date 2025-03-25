#include <stdio.h>
#include <GL/glut.h>

void display() {
    // Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw a red triangle
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
    glEnd();
    
    // Display the rendered scene
    glFlush();
}

int main(int argc, char** argv) {
    printf("Starting simple OpenGL test...\n");
    
    // Initialize GLUT
    glutInit(&argc, argv);
    
    printf("GLUT initialized\n");
    
    // Create window
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Test");
    
    printf("Window created\n");
    
    // Set clear color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Set up orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    
    // Register callback
    glutDisplayFunc(display);
    
    printf("Entering main loop...\n");
    // Enter rendering loop
    glutMainLoop();
    
    return 0;
} 