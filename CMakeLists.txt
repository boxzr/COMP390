cmake_minimum_required(VERSION 3.10)
project(Comp390_TME1)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Include directories
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Add the executables
add_executable(Program1 Program1/main.cpp)
add_executable(Program2 Program2/main.cpp)
add_executable(Program3 Program3/main.cpp)

# Link libraries
target_link_libraries(Program1 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
target_link_libraries(Program2 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
target_link_libraries(Program3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}) 