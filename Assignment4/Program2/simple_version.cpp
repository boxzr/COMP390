/**
 * Assignment 4: Program 2 - Shadows (Simplified Version)
 * 
 * This program implements a simplified version of ray tracing with shadows
 * using Windows API and OpenGL.
 */

#include <windows.h>
#include <gl/GL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <time.h>

// Window dimensions and handle
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
HWND hwnd;
HDC hdc;
HGLRC hglrc;

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
class Sphere {
public:
    Vector3 center;
    float radius;
    Color color;
    
    Sphere(const Vector3& c, float r, const Color& col) 
        : center(c), radius(r), color(col) {}
};

// Plane definition
class Plane {
public:
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

// Trace a ray through the scene - implemented according to the pseudocode
Color trace(const Ray& ray, int depth) {
    // Termination condition: maximum recursive calls
    if (depth > MAX_DEPTH) {
        return Color(0, 0, 0); // No contribution - black
    }
    
    // Use the Incident Ray to find the nearest intersection
    Intersection isect = intersect(ray);
    
    // If no intersection, return background color (black) - too far away
    if (!isect.hit) {
        return Color(0, 0, 0);
    }
    
    // Get object color at the intersection point
    Color surface_color;
    if (isect.object_type == SPHERE) {
        surface_color = spheres[isect.object_index].color;
    } else if (isect.object_type == PLANE) {
        surface_color = planes[isect.object_index].color;
        
        // Add checkerboard pattern to the floor
        if (isect.object_index == 0) {  // Floor plane
            int cx = (int)(isect.point.x + 1000) / 1;
            int cz = (int)(isect.point.z + 1000) / 1;
            if ((cx + cz) % 2 == 0) {
                surface_color = surface_color * 0.8f;
            }
        }
    }
    
    // Calculate direction to light
    Vector3 light_dir = (LIGHT_POSITION - isect.point).normalize();
    
    // Calculate view direction
    Vector3 view_dir = (CAMERA_POSITION - isect.point).normalize();
    
    // Cast a ray from the intersection to the light source - check for shadows
    bool in_shadow = isInShadow(isect.point, isect.normal);
    
    // Calculate ambient component (always present)
    Color ambient = AMBIENT_COLOR * surface_color;
    
    // Initialize local_color with ambient lighting
    Color local_color = ambient;
    
    // If not in shadow, add diffuse and specular components
    if (!in_shadow) {
        // Calculate diffuse lighting
        float diffuse = calculateDiffuse(isect.normal, light_dir);
        Color diffuse_color = surface_color * LIGHT_COLOR * diffuse;
        
        // Calculate specular lighting (with shininess based on object type)
        float shininess = isect.object_type == SPHERE ? 40.0f : 5.0f;
        float specular = calculateSpecular(isect.normal, light_dir, view_dir, shininess);
        Color specular_color = LIGHT_COLOR * specular;
        
        // Add diffuse and specular to local color
        local_color = local_color + diffuse_color + specular_color;
    }
    
    // Total color contribution is the local color
    // (Could add reflections here like in Program 1 if desired)
    Color total_color = local_color;
    
    // Clamp color values
    total_color.clamp();
    
    return total_color;
}

// Render the scene using ray tracing
void render() {
    // Create a buffer for our pixel data
    COLORREF* pixels = new COLORREF[WINDOW_WIDTH * WINDOW_HEIGHT];
    
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
            
            // Convert color to Windows COLORREF format (RGB)
            int r = static_cast<int>(pixel_color.r * 255);
            int g = static_cast<int>(pixel_color.g * 255);
            int b = static_cast<int>(pixel_color.b * 255);
            pixels[y * WINDOW_WIDTH + x] = RGB(r, g, b);
        }
    }
    
    // Create a bitmap to hold our pixel data
    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = WINDOW_WIDTH;
    bmi.bmiHeader.biHeight = -WINDOW_HEIGHT;  // Negative for top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    
    // Draw the bitmap to the screen
    StretchDIBits(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 
                 pixels, &bmi, DIB_RGB_COLORS, SRCCOPY);
    
    // Clean up
    delete[] pixels;
}

// Windows procedure for handling events
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_PAINT:
            render();
            ValidateRect(hwnd, NULL);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                DestroyWindow(hwnd);
            }
            return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

// Set up the OpenGL context
bool SetupOpenGL() {
    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    
    // Get the device context
    hdc = GetDC(hwnd);
    
    // Set the pixel format for the device context
    int format = ChoosePixelFormat(hdc, &pfd);
    if (format == 0) {
        return false;
    }
    
    if (!SetPixelFormat(hdc, format, &pfd)) {
        return false;
    }
    
    // Create the OpenGL rendering context
    hglrc = wglCreateContext(hdc);
    if (!hglrc) {
        return false;
    }
    
    // Make the rendering context current
    if (!wglMakeCurrent(hdc, hglrc)) {
        return false;
    }
    
    return true;
}

// Clean up the OpenGL context
void CleanupOpenGL() {
    wglMakeCurrent(NULL, NULL);
    if (hglrc) {
        wglDeleteContext(hglrc);
        hglrc = NULL;
    }
    if (hdc) {
        ReleaseDC(hwnd, hdc);
        hdc = NULL;
    }
}

// Main entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize the scene
    initScene();
    
    // Register the window class
    WNDCLASS wc;
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "RayTracingWindowClass";
    
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Could not register window class!", "Error", MB_OK);
        return 1;
    }
    
    // Create the window
    hwnd = CreateWindow(
        "RayTracingWindowClass",
        "Ray Tracing - Shadows",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL
    );
    
    if (!hwnd) {
        MessageBox(NULL, "Could not create window!", "Error", MB_OK);
        return 1;
    }
    
    // Set up OpenGL
    if (!SetupOpenGL()) {
        MessageBox(NULL, "Could not initialize OpenGL!", "Error", MB_OK);
        CleanupOpenGL();
        return 1;
    }
    
    // Show the window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    // Enter the message loop
    MSG msg;
    bool quit = false;
    
    // Main message loop
    while (!quit) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                quit = true;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        } else {
            render();
        }
    }
    
    // Clean up OpenGL
    CleanupOpenGL();
    
    return (int)msg.wParam;
} 