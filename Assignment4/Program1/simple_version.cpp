/**
 * Assignment 4: Program 1 - Recursive Reflection (Simplified Version)
 * 
 * This program implements a simplified version of recursive ray tracing
 * to simulate reflections on surfaces.
 */

#include <windows.h>
#include <gl/GL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Window dimensions and handle
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
HWND hwnd;
HDC hdc;
HGLRC hglrc;

// Maximum recursion depth
const int MAX_DEPTH = 5;

// Reflection coefficient for the floor
const float REFLECTION_COEFFICIENT = 0.6f;

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

// Define surface types
enum SurfaceType {
    NONE,
    FLOOR,
    WALL
};

// Intersection information
struct Intersection {
    bool hit;
    float distance;
    Vector3 point;
    Vector3 normal;
    SurfaceType surface;
    
    Intersection() : hit(false), distance(0), surface(NONE) {}
};

// Scene definitions
const Vector3 FLOOR_NORMAL(0, 1, 0);  // Floor normal (pointing up)
const Vector3 LEFT_WALL_NORMAL(1, 0, 0);   // Left wall normal (pointing right)
const Vector3 RIGHT_WALL_NORMAL(-1, 0, 0); // Right wall normal (pointing left)
const Vector3 BACK_WALL_NORMAL(0, 0, 1);   // Back wall normal (pointing forward)

// Colors
const Color FLOOR_COLOR(0.8f, 0.8f, 0.8f);   // Light gray floor
const Color LEFT_WALL_COLOR(1.0f, 0.0f, 0.0f);   // Red left wall
const Color RIGHT_WALL_COLOR(0.0f, 1.0f, 0.0f);  // Green right wall
const Color BACK_WALL_COLOR(0.0f, 0.0f, 1.0f);   // Blue back wall
const Color AMBIENT_COLOR(0.2f, 0.2f, 0.2f);  // Ambient light

// Light source
const Vector3 LIGHT_POSITION(0, 5, 2);  // Light above and slightly in front
const Color LIGHT_COLOR(1.0f, 1.0f, 1.0f);  // White light

// Camera position (viewing from)
const Vector3 CAMERA_POSITION(0, 1, 5);  // Position slightly above floor, away from scene

// Check for intersection with the floor
bool intersectFloor(const Ray& ray, float& t) {
    // Floor is at y = 0
    if (fabs(ray.direction.y) < 0.0001f) {
        return false;  // Ray is parallel to the floor
    }
    
    t = -ray.origin.y / ray.direction.y;
    return t > 0.0001f;  // Return true if intersection is in front of the ray
}

// Check for intersection with walls
bool intersectWalls(const Ray& ray, float& t, Vector3& normal) {
    float t_min = INFINITY;
    bool hit = false;
    
    // Left wall at x = -5
    if (fabs(ray.direction.x) > 0.0001f) {
        float t_left = (-5 - ray.origin.x) / ray.direction.x;
        if (t_left > 0.0001f && t_left < t_min) {
            Vector3 p = ray.origin + ray.direction * t_left;
            if (p.y >= 0 && p.y <= 5 && p.z >= -5 && p.z <= 0) {
                t_min = t_left;
                normal = LEFT_WALL_NORMAL;
                hit = true;
            }
        }
    }
    
    // Right wall at x = 5
    if (fabs(ray.direction.x) > 0.0001f) {
        float t_right = (5 - ray.origin.x) / ray.direction.x;
        if (t_right > 0.0001f && t_right < t_min) {
            Vector3 p = ray.origin + ray.direction * t_right;
            if (p.y >= 0 && p.y <= 5 && p.z >= -5 && p.z <= 0) {
                t_min = t_right;
                normal = RIGHT_WALL_NORMAL;
                hit = true;
            }
        }
    }
    
    // Back wall at z = -5
    if (fabs(ray.direction.z) > 0.0001f) {
        float t_back = (-5 - ray.origin.z) / ray.direction.z;
        if (t_back > 0.0001f && t_back < t_min) {
            Vector3 p = ray.origin + ray.direction * t_back;
            if (p.y >= 0 && p.y <= 5 && p.x >= -5 && p.x <= 5) {
                t_min = t_back;
                normal = BACK_WALL_NORMAL;
                hit = true;
            }
        }
    }
    
    if (hit) {
        t = t_min;
    }
    return hit;
}

// Find the closest intersection of a ray with the scene
Intersection intersect(const Ray& ray) {
    Intersection result;
    result.hit = false;
    
    // Check intersection with the floor
    float t_floor;
    if (intersectFloor(ray, t_floor)) {
        Vector3 p = ray.origin + ray.direction * t_floor;
        
        // Check if the point is within our scene boundaries
        if (p.x >= -5 && p.x <= 5 && p.z >= -5 && p.z <= 0) {
            result.hit = true;
            result.distance = t_floor;
            result.point = p;
            result.normal = FLOOR_NORMAL;
            result.surface = FLOOR;
        }
    }
    
    // Check intersection with walls
    float t_wall;
    Vector3 wall_normal;
    if (intersectWalls(ray, t_wall, wall_normal)) {
        if (!result.hit || t_wall < result.distance) {
            result.hit = true;
            result.distance = t_wall;
            result.point = ray.origin + ray.direction * t_wall;
            result.normal = wall_normal;
            result.surface = WALL;
        }
    }
    
    return result;
}

// Calculate the reflection direction
Vector3 reflect(const Vector3& incident, const Vector3& normal) {
    return incident - normal * (2 * incident.dot(normal));
}

// Calculate diffuse lighting
float calculateDiffuse(const Vector3& normal, const Vector3& lightDir) {
    float dot = normal.dot(lightDir);
    return dot > 0 ? dot : 0;
}

// Recursive ray tracing function - implemented according to the pseudocode
Color trace(const Ray& ray, int depth) {
    // Termination condition: maximum recursive calls
    if (depth > MAX_DEPTH) {
        return Color(0, 0, 0); // No contribution - black
    }
    
    // Use the Incident Ray to find the nearest intersection
    Intersection intersect_result = intersect(ray);
    
    // If no intersection, return black (background color) - too far away
    if (!intersect_result.hit) {
        return Color(0, 0, 0);
    }
    
    // Calculate the color at the intersection point
    Color result_color(0, 0, 0);
    
    // Cast a ray from the intersection to the light source
    Vector3 light_dir = (LIGHT_POSITION - intersect_result.point).normalize();
    
    // Basic ambient color
    Color ambient = AMBIENT_COLOR;
    
    // Determine surface color based on the hit surface
    Color surface_color;
    if (intersect_result.surface == FLOOR) {
        surface_color = FLOOR_COLOR;
    } else if (intersect_result.surface == WALL) {
        // Determine which wall was hit based on the normal
        if (intersect_result.normal.x > 0.9f) {  // Left wall
            surface_color = LEFT_WALL_COLOR;
        } else if (intersect_result.normal.x < -0.9f) {  // Right wall
            surface_color = RIGHT_WALL_COLOR;
        } else {  // Back wall
            surface_color = BACK_WALL_COLOR;
        }
    }
    
    // Check if there's anything between intersection point and the light
    // Create a shadow ray (offset slightly to avoid self-intersection)
    Ray shadow_ray(intersect_result.point + intersect_result.normal * 0.0001f, light_dir);
    
    // Distance to light
    float dist_to_light = (LIGHT_POSITION - intersect_result.point).length();
    
    // Check for shadows
    Intersection shadow_isect = intersect(shadow_ray);
    bool in_shadow = shadow_isect.hit && shadow_isect.distance < dist_to_light;
    
    // Calculate local color
    Color local_color = ambient * surface_color; // Start with ambient
    
    // If not in shadow, add diffuse component
    if (!in_shadow) {
        float diffuse = calculateDiffuse(intersect_result.normal, light_dir);
        Color diffuse_color = surface_color * LIGHT_COLOR * diffuse;
        local_color = local_color + diffuse_color;
    }
    
    // Total color starts with local color
    Color total_color = local_color;
    
    // If the surface is reflective (floor) and we haven't reached max depth
    if (intersect_result.surface == FLOOR && depth < MAX_DEPTH) {
        // Calculate reflection direction
        Vector3 reflection_dir = reflect(ray.direction, intersect_result.normal);
        
        // Create the reflection ray
        Ray reflection_ray(intersect_result.point + intersect_result.normal * 0.0001f, reflection_dir);
        
        // Recursively trace the reflection ray to get reflective color
        Color reflection_color = trace(reflection_ray, depth + 1);
        
        // Mix the local color with the reflection color based on reflection coefficient
        total_color = local_color * (1 - REFLECTION_COEFFICIENT) + 
                      reflection_color * REFLECTION_COEFFICIENT;
    }
    
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
        "Ray Tracing - Recursive Reflection",
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