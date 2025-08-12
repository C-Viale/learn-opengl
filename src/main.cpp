#include <glad/glad.h>
//
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader/shader.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void renderTriangle();

const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

int main() {
  // initialize GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // initialize window
  GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "HELLO WORLD", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  // initialize GLAD before calling any OpenGL function
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  unsigned int shaderProgram = buildShaderProgram();

  float vertices[] = {
      -0.5f, -0.25f, 0.0f,  // 0 - left corner
      -0.25f, 0.25f, 0.0f,  // 1 - left top
      0.0f, -0.25f, 0.0f,   // 2 - center
      0.5f, -0.25f, 0.0f,   // 3 - right corner
      0.25f, 0.25f, 0.0f,   // 4 - right top

  };

  unsigned int indices[] = {
      0, 1, 2,  // first triangle
      4, 3, 2   // second triangle
  };

  unsigned int VBO, VAO, EBO;

  glGenVertexArrays(1, &VAO);  // generate unique vertex array identifier
  glGenBuffers(1, &VBO);       // generate unique vertex buffer identifier
  glGenBuffers(1, &EBO);       // generate unique vertex element identifier

  /*
     The Vertex Array Object (VAO) is bound like the Vertex Buffer Object (VBO).
     Any vertex attribute calls after the bound will be stored inside the VAO.
     Because of this, whe bind the VAO before the VBO
  */

  // 1. bind the VAO first
  glBindVertexArray(VAO);

  // 2. set vertex buffers
  glBindBuffer(GL_ARRAY_BUFFER, VBO);                                         // bind VBO to GL_ARRAY_BUFFER
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // store data from vertices to the GL_ARRAY_BUFFER in the GPU

  // 3. set vertex element bufer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);                                       // bind the EBO to GL_ELEMENT_ARRAY_BUFFER
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);  // store indices  to the GL_ELEMENT_ARRAY_BUFFER in the GPU

  // 4. configure vertex attributes;
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // wireframe mode
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);  // this will swap the color buffer used to render and show it as output to the screen
    glfwPollEvents();         // this checks if any events are triggered, updates the window state and execute callbacks
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteVertexArrays(1, &VBO);
  glDeleteProgram(shaderProgram);
  glfwTerminate();
  return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
