/**
 * Assignment 4: Program 2 - Shadows
 * 
 * This program implements ray tracing with shadow effects.
 * 
 * The render() method has been modified to implement a ray tracing algorithm
 * that checks if intersection points can see the light source to determine
 * if they are in shadow.
 */

#define GLUT_DISABLE_ATEXIT_HACK
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <GL/glut.h>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Maximum recursion depth
const int MAX_DEPTH = 3;

// Define a simple 3D vector class
class Vector3 {
public:
    float x, y, z;
    
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    // Vector addition
    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }
    
    // Vector subtraction
    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    
    // Scalar multiplication
    Vector3 operator*(float s) const {
        return Vector3(x * s, y * s, z * s);
    }
    
    // Dot product
    float dot(const Vector3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }
    
    // Cross product
    Vector3 cross(const Vector3& v) const {
        return Vector3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }
    
    // Normalize
    Vector3 normalize() const {
        float mag = sqrt(x * x + y * y + z * z);
        if (mag > 0.0001f) {
            return Vector3(x / mag, y / mag, z / mag);
        }
        return *this;
    }
    
    // Length
    float length() const {
        return sqrt(x * x + y * y + z * z);
    }
};

// Define a ray
struct Ray {
    Vector3 origin;
    Vector3 direction;
    
    Ray(const Vector3& o, const Vector3& d) : origin(o), direction(d.normalize()) {}
};

// Define a color
struct Color {
    float r, g, b;
    
    Color() : r(0), g(0), b(0) {}
    Color(float r, float g, float b) : r(r), g(g), b(b) {}
    
    // Color addition
    Color operator+(const Color& c) const {
        return Color(r + c.r, g + c.g, b + c.b);
    }
    
    // Color scaling
    Color operator*(float s) const {
        return Color(r * s, g * s, b * s);
    }
    
    // Color multiplication (component-wise)
    Color operator*(const Color& c) const {
        return Color(r * c.r, g * c.g, b * c.b);
    }
    
    // Clamp color values between 0 and 1
    void clamp() {
        r = r > 1.0f ? 1.0f : (r < 0.0f ? 0.0f : r);
        g = g > 1.0f ? 1.0f : (g < 0.0f ? 0.0f : g);
        b = b > 1.0f ? 1.0f : (b < 0.0f ? 0.0f : b);
    }
};

// Define object types
enum ObjectType {
    NONE,
    SPHERE,
    PLANE
};

// Intersection information
struct Intersection {
    bool hit;
    float distance;
    Vector3 point;
    Vector3 normal;
    ObjectType object_type;
    int object_index;  // Index to identify which object was hit
    
    Intersection() : hit(false), distance(0), object_type(NONE), object_index(-1) {}
};

// Sphere definition
struct Sphere {
    Vector3 center;
    float radius;
    Color color;
    
    Sphere(const Vector3& c, float r, const Color& col) 
        : center(c), radius(r), color(col) {}
};

// Plane definition
struct Plane {
    Vector3 point;  // A point on the plane
    Vector3 normal; // Normal to the plane
    Color color;
    
    Plane(const Vector3& p, const Vector3& n, const Color& c)
        : point(p), normal(n.normalize()), color(c) {}
};

// Scene objects
std::vector<Sphere> spheres;
std::vector<Plane> planes;

// Colors
const Color AMBIENT_COLOR(0.2f, 0.2f, 0.2f);  // Ambient light

// Light source
const Vector3 LIGHT_POSITION(2, 5, 3);  // Light above and to the side
const Color LIGHT_COLOR(1.0f, 1.0f, 1.0f);  // White light

// Camera position (viewing from)
const Vector3 CAMERA_POSITION(0, 1, 5);  // Position slightly above floor, away from scene

// Initialize the scene with objects
void initScene() {
    // Add spheres
    spheres.push_back(Sphere(Vector3(-1.5f, 1.0f, -2.0f), 1.0f, Color(1.0f, 0.0f, 0.0f)));  // Red sphere
    spheres.push_back(Sphere(Vector3(1.5f, 0.5f, -1.0f), 0.5f, Color(0.0f, 1.0f, 0.0f)));   // Green sphere
    spheres.push_back(Sphere(Vector3(0.0f, 0.5f, -3.0f), 0.5f, Color(0.0f, 0.0f, 1.0f)));   // Blue sphere
    
    // Add planes
    planes.push_back(Plane(Vector3(0, 0, 0), Vector3(0, 1, 0), Color(0.8f, 0.8f, 0.8f)));   // Floor
    planes.push_back(Plane(Vector3(0, 0, -5), Vector3(0, 0, 1), Color(0.6f, 0.6f, 0.8f)));  // Back wall
}

// Check for intersection with a sphere
bool intersectSphere(const Ray& ray, const Sphere& sphere, float& t) {
    Vector3 oc = ray.origin - sphere.center;
    float a = ray.direction.dot(ray.direction);
    float b = 2.0f * oc.dot(ray.direction);
    float c = oc.dot(oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0) {
        return false;
    }
    
    float t0 = (-b - sqrt(discriminant)) / (2.0f * a);
    float t1 = (-b + sqrt(discriminant)) / (2.0f * a);
    
    // Find the nearest positive intersection
    if (t0 > 0.0001f) {
        t = t0;
        return true;
    }
    
    if (t1 > 0.0001f) {
        t = t1;
        return true;
    }
    
    return false;
}

// Check for intersection with a plane
bool intersectPlane(const Ray& ray, const Plane& plane, float& t) {
    float denom = ray.direction.dot(plane.normal);
    
    // Check if ray is parallel to the plane
    if (fabs(denom) < 0.0001f) {
        return false;
    }
    
    Vector3 p0l0 = plane.point - ray.origin;
    t = p0l0.dot(plane.normal) / denom;
    
    return t > 0.0001f;
}

// Find the closest intersection of a ray with the scene
Intersection intersect(const Ray& ray) {
    Intersection result;
    result.hit = false;
    result.distance = INFINITY;
    
    // Check intersection with spheres
    for (size_t i = 0; i < spheres.size(); i++) {
        float t;
        if (intersectSphere(ray, spheres[i], t)) {
            if (t < result.distance) {
                result.hit = true;
                result.distance = t;
                result.point = ray.origin + ray.direction * t;
                result.normal = (result.point - spheres[i].center).normalize();
                result.object_type = SPHERE;
                result.object_index = i;
            }
        }
    }
    
    // Check intersection with planes
    for (size_t i = 0; i < planes.size(); i++) {
        float t;
        if (intersectPlane(ray, planes[i], t)) {
            if (t < result.distance) {
                result.hit = true;
                result.distance = t;
                result.point = ray.origin + ray.direction * t;
                result.normal = planes[i].normal;
                result.object_type = PLANE;
                result.object_index = i;
            }
        }
    }
    
    return result;
}

// Check if a point is in shadow
bool isInShadow(const Vector3& point, const Vector3& normal) {
    // Calculate direction to light
    Vector3 light_dir = (LIGHT_POSITION - point).normalize();
    
    // Create a shadow ray (offset slightly to avoid self-intersection)
    Ray shadow_ray(point + normal * 0.0001f, light_dir);
    
    // Distance to light
    float dist_to_light = (LIGHT_POSITION - point).length();
    
    // Check for intersection with scene objects
    Intersection shadow_isect = intersect(shadow_ray);
    
    // If we hit something closer than the light, we are in shadow
    return shadow_isect.hit && shadow_isect.distance < dist_to_light;
}

// Calculate diffuse lighting
float calculateDiffuse(const Vector3& normal, const Vector3& light_dir) {
    float dot = normal.dot(light_dir);
    return dot > 0 ? dot : 0;
}

// Calculate specular lighting
float calculateSpecular(const Vector3& normal, const Vector3& light_dir, const Vector3& view_dir, float shininess) {
    // Calculate the half vector between light and view directions
    Vector3 half_vec = (light_dir + view_dir).normalize();
    
    float dot = normal.dot(half_vec);
    return dot > 0 ? pow(dot, shininess) : 0;
}

// Trace a ray through the scene
Color trace(const Ray& ray, int depth) {
    // Base case: maximum recursion depth reached
    if (depth > MAX_DEPTH) {
        return Color(0, 0, 0);
    }
    
    // Find intersection with the scene
    Intersection isect = intersect(ray);
    
    // If no intersection, return background color (black)
    if (!isect.hit) {
        return Color(0, 0, 0);
    }
    
    // Get object color
    Color object_color;
    if (isect.object_type == SPHERE) {
        object_color = spheres[isect.object_index].color;
    } else if (isect.object_type == PLANE) {
        object_color = planes[isect.object_index].color;
        
        // Add checkerboard pattern to the floor
        if (isect.object_index == 0) {  // Floor plane
            int cx = (int)(isect.point.x + 1000) / 1;
            int cz = (int)(isect.point.z + 1000) / 1;
            if ((cx + cz) % 2 == 0) {
                object_color = object_color * 0.8f;
            }
        }
    }
    
    // Calculate direction to light
    Vector3 light_dir = (LIGHT_POSITION - isect.point).normalize();
    
    // Calculate view direction
    Vector3 view_dir = (CAMERA_POSITION - isect.point).normalize();
    
    // Check if the point is in shadow
    bool in_shadow = isInShadow(isect.point, isect.normal);
    
    // Calculate ambient component
    Color ambient = AMBIENT_COLOR * object_color;
    
    // Initialize result with ambient lighting (always present)
    Color result = ambient;
    
    // If not in shadow, add diffuse and specular components
    if (!in_shadow) {
        // Calculate diffuse lighting
        float diffuse = calculateDiffuse(isect.normal, light_dir);
        Color diffuse_color = object_color * LIGHT_COLOR * diffuse;
        
        // Calculate specular lighting (with shininess based on object type)
        float shininess = isect.object_type == SPHERE ? 40.0f : 5.0f;
        float specular = calculateSpecular(isect.normal, light_dir, view_dir, shininess);
        Color specular_color = LIGHT_COLOR * specular;
        
        // Add diffuse and specular to the result
        result = result + diffuse_color + specular_color;
    }
    
    // Clamp color values
    result.clamp();
    
    return result;
}

// Render the scene using ray tracing
void render() {
    // Set up the viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Render the image
    glBegin(GL_POINTS);
    
    // Calculate the aspect ratio
    float aspect_ratio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    
    // For each pixel in the window
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            // Calculate normalized device coordinates
            float ndc_x = (2.0f * x / WINDOW_WIDTH - 1.0f) * aspect_ratio;
            float ndc_y = 1.0f - 2.0f * y / WINDOW_HEIGHT;
            
            // Create a ray from the camera through this pixel
            Vector3 ray_dir(ndc_x, ndc_y, -1.0f);  // -1 because we're looking down the -z axis
            ray_dir = ray_dir.normalize();
            
            Ray ray(CAMERA_POSITION, ray_dir);
            
            // Trace the ray and get the color
            Color pixel_color = trace(ray, 0);
            
            // Set the pixel color
            glColor3f(pixel_color.r, pixel_color.g, pixel_color.b);
            glVertex2i(x, y);
        }
    }
    
    glEnd();
    glutSwapBuffers();
}

// Display callback function
void display() {
    render();
}

// Keyboard callback function
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {  // ESC key
        exit(0);
    }
}

// Main function
int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Ray Tracing - Shadows");
    
    // Set up viewport and projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Initialize the scene
    initScene();
    
    // Register callbacks
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    
    // Set the clear color
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    // Enter the GLUT main loop
    glutMainLoop();
    
    return 0;
} 