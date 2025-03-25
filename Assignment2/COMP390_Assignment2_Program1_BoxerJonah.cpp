/**
 * program 1: object modeling
 * 
 * displays a house model with ground, cube body and pyramid top
 * using opengl display lists
 */

#include <GL/glut.h>

// display list id
GLuint houseList;

// creates a display list for the house model
void createHouseList() {
    // start a new display list
    houseList = glGenLists(1);
    glNewList(houseList, GL_COMPILE);

    // draw ground (green)
    glColor3f(0.0f, 0.8f, 0.0f);
    glBegin(GL_QUADS);
        glVertex3f(-1.0f, 0.0f, -0.5f);  // far left
        glVertex3f(2.0f, 0.0f, -0.5f);   // far right
        glVertex3f(2.0f, 0.0f, 2.5f);    // near right
        glVertex3f(-1.0f, 0.0f, 2.5f);   // near left
    glEnd();

    // draw cube body (red)
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
    
    // draw pyramid top (blue)
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

void display() {
    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // set up view
    glLoadIdentity();
    gluLookAt(3.0, 3.0, 3.0,   // eye position
              0.5, 0.5, 0.5,   // look at position
              0.0, 1.0, 0.0);  // up vector
    
    // draw the house
    glCallList(houseList);
    
    // swap buffers
    glutSwapBuffers();
}

void reshape(int w, int h) {
    // prevent division by zero
    if (h == 0) h = 1;
    
    // set viewport to window dimensions
    glViewport(0, 0, w, h);
    
    // set perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 0.1f, 100.0f);
    
    // switch back to modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
    // exit on escape key
    if (key == 27) { // ESC
        exit(0);
    }
    
    // toggle between solid and wireframe
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

void init() {
    // set clear color to light gray
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    
    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // create display list
    createHouseList();
}

int main(int argc, char** argv) {
    // initialize GLUT
    glutInit(&argc, argv);
    
    // use double buffering and depth testing
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    // set window size and position
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    
    // create window with title
    glutCreateWindow("Athabasca University - Comp390 TME 2 Program 1 Demo");
    
    // init OpenGL state
    init();
    
    // register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    
    // enter main loop
    glutMainLoop();
    
    return 0;
} 