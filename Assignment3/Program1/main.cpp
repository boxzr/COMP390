/**
 * Assignment 3: Program 1 - Lighting elements and surface materials
 * 
 * This program demonstrates the use of OpenGL lighting features to illuminate
 * a sphere with three light sources. Users can select different light colors
 * and surface materials through a right-click menu system.
 */

#include <GL/glut.h>

// Light positions
GLfloat light0_position[] = {-5.0, 5.0, 5.0, 1.0};  // Top left
GLfloat light1_position[] = {5.0, 5.0, 5.0, 1.0};   // Top right
GLfloat light2_position[] = {0.0, 0.0, 10.0, 1.0};  // Behind viewer

// Light colors
GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
GLfloat green_light[] = {0.2, 1.0, 0.2, 1.0};
GLfloat red_light[] = {1.0, 0.4, 0.4, 1.0};
GLfloat blue_light[] = {0.2, 0.2, 1.0, 1.0};

// Gold material properties
GLfloat gold_ambient[] = {0.24725, 0.1995, 0.0745, 1.0};
GLfloat gold_diffuse[] = {0.75164, 0.60648, 0.22658, 1.0};
GLfloat gold_specular[] = {0.628281, 0.555802, 0.366065, 1.0};
GLfloat gold_shininess[] = {51.2};

// Silver material properties
GLfloat silver_ambient[] = {0.19225, 0.19225, 0.19225, 1.0};
GLfloat silver_diffuse[] = {0.50754, 0.50754, 0.50754, 1.0};
GLfloat silver_specular[] = {0.508273, 0.508273, 0.508273, 1.0};
GLfloat silver_shininess[] = {51.2};

// Copper material properties
GLfloat copper_ambient[] = {0.2295, 0.08825, 0.0275, 1.0};
GLfloat copper_diffuse[] = {0.5508, 0.2118, 0.066, 1.0};
GLfloat copper_specular[] = {0.580594, 0.223257, 0.0695701, 1.0};
GLfloat copper_shininess[] = {51.2};

// Current state
GLfloat* current_light0 = white_light;
GLfloat* current_light1 = red_light;
GLfloat* current_light2 = blue_light;
int current_material = 0; // 0 = gold, 1 = silver, 2 = copper
bool light0_on = true;
bool light1_on = true;
bool light2_on = true;
float sphere_rotation = 0.0f;

// Function to set material properties
void setMaterial(int material) {
    switch (material) {
        case 0: // Gold
            glMaterialfv(GL_FRONT, GL_AMBIENT, gold_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, gold_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, gold_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, gold_shininess);
            break;
        case 1: // Silver
            glMaterialfv(GL_FRONT, GL_AMBIENT, silver_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, silver_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, silver_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, silver_shininess);
            break;
        case 2: // Copper
            glMaterialfv(GL_FRONT, GL_AMBIENT, copper_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, copper_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, copper_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, copper_shininess);
            break;
    }
}

// Display function
void display() {
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Reset transformations
    glLoadIdentity();
    
    // Set the camera position
    gluLookAt(0.0, 0.0, 5.0,  // Eye position
              0.0, 0.0, 0.0,  // Look-at position
              0.0, 1.0, 0.0); // Up vector
    
    // Apply lighting
    glPushMatrix();
    
    // Set lights with current color
    if (light0_on) {
        glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, current_light0);
        glLightfv(GL_LIGHT0, GL_SPECULAR, current_light0);
    } else {
        // Turn off light
        GLfloat no_light[] = {0.0, 0.0, 0.0, 1.0};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, no_light);
        glLightfv(GL_LIGHT0, GL_SPECULAR, no_light);
    }
    
    if (light1_on) {
        glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, current_light1);
        glLightfv(GL_LIGHT1, GL_SPECULAR, current_light1);
    } else {
        // Turn off light
        GLfloat no_light[] = {0.0, 0.0, 0.0, 1.0};
        glLightfv(GL_LIGHT1, GL_DIFFUSE, no_light);
        glLightfv(GL_LIGHT1, GL_SPECULAR, no_light);
    }
    
    if (light2_on) {
        glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, current_light2);
        glLightfv(GL_LIGHT2, GL_SPECULAR, current_light2);
    } else {
        // Turn off light
        GLfloat no_light[] = {0.0, 0.0, 0.0, 1.0};
        glLightfv(GL_LIGHT2, GL_DIFFUSE, no_light);
        glLightfv(GL_LIGHT2, GL_SPECULAR, no_light);
    }
    
    // Rotate the sphere
    glRotatef(sphere_rotation, 0.0, 1.0, 0.0);
    
    // Set material properties
    setMaterial(current_material);
    
    // Draw sphere
    glutSolidSphere(1.0, 50, 50);
    
    glPopMatrix();
    glutSwapBuffers();
}

// Reshape function
void reshape(int w, int h) {
    // Prevent division by zero
    if (h == 0) h = 1;
    
    // Set viewport to window dimensions
    glViewport(0, 0, w, h);
    
    // Set perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 0.1f, 100.0f);
    
    // Switch back to modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Timer function for animation
void timer(int value) {
    // Update sphere rotation
    sphere_rotation += 2.0f;
    if (sphere_rotation > 360.0f) {
        sphere_rotation -= 360.0f;
    }
    
    glutPostRedisplay();
    glutTimerFunc(30, timer, 0);
}

// Menu callbacks
void lightSourceMenu0(int option) {
    switch (option) {
        case 0: current_light0 = white_light; break;
        case 1: current_light0 = red_light; break;
        case 2: current_light0 = green_light; break;
        case 3: current_light0 = blue_light; break;
    }
    glutPostRedisplay();
}

void lightSourceMenu1(int option) {
    switch (option) {
        case 0: current_light1 = white_light; break;
        case 1: current_light1 = red_light; break;
        case 2: current_light1 = green_light; break;
        case 3: current_light1 = blue_light; break;
    }
    glutPostRedisplay();
}

void lightSourceMenu2(int option) {
    switch (option) {
        case 0: current_light0 = white_light; break;
        case 1: current_light2 = red_light; break;
        case 2: current_light2 = green_light; break;
        case 3: current_light2 = blue_light; break;
    }
    glutPostRedisplay();
}

void toggleLightMenu(int option) {
    switch (option) {
        case 0: light0_on = !light0_on; break;
        case 1: light1_on = !light1_on; break;
        case 2: light2_on = !light2_on; break;
    }
    glutPostRedisplay();
}

void materialMenu(int option) {
    current_material = option;
    glutPostRedisplay();
}

void mainMenu(int option) {
    if (option == 0) {
        exit(0);
    }
}

// Initialize menus
void createMenus() {
    int lightMenu0 = glutCreateMenu(lightSourceMenu0);
    glutAddMenuEntry("White", 0);
    glutAddMenuEntry("Red", 1);
    glutAddMenuEntry("Green", 2);
    glutAddMenuEntry("Blue", 3);
    
    int lightMenu1 = glutCreateMenu(lightSourceMenu1);
    glutAddMenuEntry("White", 0);
    glutAddMenuEntry("Red", 1);
    glutAddMenuEntry("Green", 2);
    glutAddMenuEntry("Blue", 3);
    
    int lightMenu2 = glutCreateMenu(lightSourceMenu2);
    glutAddMenuEntry("White", 0);
    glutAddMenuEntry("Red", 1);
    glutAddMenuEntry("Green", 2);
    glutAddMenuEntry("Blue", 3);
    
    int toggleMenu = glutCreateMenu(toggleLightMenu);
    glutAddMenuEntry("Toggle Light 1 (Top Left)", 0);
    glutAddMenuEntry("Toggle Light 2 (Top Right)", 1);
    glutAddMenuEntry("Toggle Light 3 (Behind Viewer)", 2);
    
    int matMenu = glutCreateMenu(materialMenu);
    glutAddMenuEntry("Gold", 0);
    glutAddMenuEntry("Silver", 1);
    glutAddMenuEntry("Copper", 2);
    
    int mainMenuId = glutCreateMenu(mainMenu);
    glutAddSubMenu("Light 1 Color", lightMenu0);
    glutAddSubMenu("Light 2 Color", lightMenu1);
    glutAddSubMenu("Light 3 Color", lightMenu2);
    glutAddSubMenu("Toggle Lights", toggleMenu);
    glutAddSubMenu("Material", matMenu);
    glutAddMenuEntry("Exit", 0);
    
    // Attach menu to right mouse button
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Keyboard callback
void keyboard(unsigned char key, int x, int y) {
    // Exit on escape key
    if (key == 27) { // ESC
        exit(0);
    }
    
    // Rotate view left/right
    if (key == 'a' || key == 'A') {
        sphere_rotation -= 5.0f;
        glutPostRedisplay();
    }
    if (key == 'd' || key == 'D') {
        sphere_rotation += 5.0f;
        glutPostRedisplay();
    }
}

// Initialization function
void init() {
    // Set background color to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    
    // Set ambient light
    GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    
    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    
    // Set material properties
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    // Set smooth shading
    glShadeModel(GL_SMOOTH);
    
    // Initialize the menu system
    createMenus();
    
    // Start animation timer
    glutTimerFunc(30, timer, 0);
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    
    // Use double buffering and depth testing
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    // Set window size and position
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    
    // Create window with title
    glutCreateWindow("Assignment 3 - Program 1: Lighting and Materials");
    
    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    
    // Initialize rendering
    init();
    
    // Enter main loop
    glutMainLoop();
    
    return 0;
} 