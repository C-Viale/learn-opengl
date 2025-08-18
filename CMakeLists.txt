cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 11)

# Library paths
set(GLFW_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libraries/GLFW/lib/libglfw3.a")
set(GLFW_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libraries/GLFW/include")
set(GLAD_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libraries/GLAD/include")
set(GLM_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libraries/glm/include")

# Find OpenGL
find_package(OpenGL REQUIRED)

# Your executable (include glad.c as a source file)
add_executable(main 
  src/main.cpp
  src/glad.c
  src/shader.cpp
  src/stb_image.cpp
)

# Link libraries
target_link_libraries(main ${GLFW_LIBRARY_PATH})
target_link_libraries(main ${OPENGL_LIBRARIES})
target_link_libraries(main "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")

# Include directories
target_include_directories(main PRIVATE 
  ${GLFW_INCLUDE_PATH}
  ${GLAD_INCLUDE_PATH}
  ${GLM_INCLUDE_PATH}
)