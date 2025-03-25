/**
 * program 2: 3D transformation and projection
 * 
 * creates a scene with multiple houses using 3D transformations
 * including rotation, translation and scaling
 */

#include <GL/glut.h>
#include <math.h>

// display list id
GLuint houseList;

// rotation angles for animation
float rotationAngle = 0.0f;

// create a display list for the basic house model
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

// draw the large ground for the scene
void drawGround() {
    // draw ground (grey)
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
        glVertex3f(-8.0f, -0.1f, -8.0f);  // far left
        glVertex3f(8.0f, -0.1f, -8.0f);   // far right
        glVertex3f(8.0f, -0.1f, 8.0f);    // near right
        glVertex3f(-8.0f, -0.1f, 8.0f);   // near left
    glEnd();
}

// draw a house with transformations
void drawHouse(float x, float z, float scale, float rotY) {
    glPushMatrix();
    
    // apply transformations in order:
    // 1. translate to position
    glTranslatef(x, 0.0f, z);
    
    // 2. rotate around y-axis
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    
    // 3. scale the house
    glScalef(scale, scale, scale);
    
    // draw the house using the display list
    glCallList(houseList);
    
    glPopMatrix();
}

// draw the entire scene with multiple houses
void drawScene() {
    // draw the ground first
    drawGround();
    
    // draw houses in a circle
    const int numHouses = 8;
    const float radius = 6.0f;
    
    for (int i = 0; i < numHouses; i++) {
        // calculate position on circle
        float angle = (float)i * (2.0f * 3.14159f / numHouses);
        float x = radius * cosf(angle);
        float z = radius * sinf(angle);
        
        // calculate scaling (vary sizes a bit)
        float scale = 0.7f + 0.3f * sinf(angle * 2.0f);
        
        // calculate rotation (face outward + some variation)
        float rotY = (angle * 180.0f / 3.14159f) + 180.0f;
        
        // draw the house
        drawHouse(x, z, scale, rotY);
    }
    
    // draw a house in the center
    drawHouse(0.0f, 0.0f, 1.0f, rotationAngle);
}

void display() {
    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // set up view
    glLoadIdentity();
    
    // position camera (eye, center, up)
    gluLookAt(12.0f, 8.0f, 12.0f,   // eye position
              0.0f, 0.0f, 0.0f,     // look at position
              0.0f, 1.0f, 0.0f);    // up vector
    
    // draw the scene
    drawScene();
    
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

void timer(int value) {
    // update rotation angle
    rotationAngle += 1.0f;
    if (rotationAngle > 360.0f) {
        rotationAngle -= 360.0f;
    }
    
    // redraw scene
    glutPostRedisplay();
    
    // reset timer for next frame
    glutTimerFunc(30, timer, 0);
}

void init() {
    // set clear color to sky blue
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    
    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // create display list for house
    createHouseList();
    
    // start animation timer
    glutTimerFunc(30, timer, 0);
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
    glutCreateWindow("Athabasca University - Comp390 TME 2 Program 2 Demo");
    
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