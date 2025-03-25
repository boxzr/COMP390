#include <GL/glut.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the display buffer
    glBegin(GL_TRIANGLES);        // Start drawing a triangle
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex2f(-0.5f, -0.5f);
        glColor3f(0.0f, 1.0f, 0.0f); // Green
        glVertex2f(0.5f, -0.5f);
        glColor3f(0.0f, 0.0f, 1.0f); // Blue
        glVertex2f(0.0f, 0.5f);
    glEnd();
    glFlush(); // Render now
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                          // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);    // Set display mode
    glutInitWindowSize(500, 500);                   // Set window size
    glutInitWindowPosition(100, 100);               // Set window position
    glutCreateWindow("GLUT Test");                  // Create window with title
    glutDisplayFunc(display);                       // Register display callback
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);           // Set clear color to black
    glutMainLoop();                                 // Enter the event-processing loop
    return 0;
}
