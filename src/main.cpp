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

  unsigned int blueShaderProgram = buildColorShaderProgram(SHADER_COLOR::blue);
  unsigned int redShaderProgram = buildColorShaderProgram(SHADER_COLOR::red);
  unsigned int greenShaderProgram = buildColorShaderProgram(SHADER_COLOR::green);

  float firstTriangleVertices[] = {
      -0.25f, 0.0f, 0.0f,  // 0 left
      0.25f, 0.0f, 0.0f,   // 1 right
      0.0f, 0.5f, 0.0f,    // 2 top
  };

  float secondTriangleVertices[] = {
      -0.25f, 0.0f, 0.0f,  // 0 top
      -0.5f, -0.5f, 0.0f,  // 1 left
      0.0f, -0.5f, 0.0f,   // 2 right
  };

  float thirdTriangleVertices[] = {
      0.25f, 0.0f, 0.0f,  // 0 top
      0.0f, -0.5f, 0.0f,  // 1 left
      0.5f, -0.5f, 0.0f,  // 1 right
  };

  unsigned int VAOs[3], VBOs[3];

  glGenVertexArrays(3, VAOs);
  glGenBuffers(3, VBOs);

  /*
     The Vertex Array Object (VAO) is bound like the Vertex Buffer Object (VBO).
     Any vertex attribute calls after the bound will be stored inside the VAO.
     Because of this, whe bind the VAO before the VBO
  */

  glBindVertexArray(VAOs[0]);                                                                           // 1. bind VAO first
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);                                                               // 2. bind VBO to GL_ARRAY_BUFFER
  glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangleVertices), firstTriangleVertices, GL_STATIC_DRAW);  // 3. set vertex data to the buffer
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);                         // 4. configure vertex attributes
  glEnableVertexAttribArray(0);                                                                         // 5. enable vertex attribute at location 0

  glBindVertexArray(VAOs[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangleVertices), secondTriangleVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAOs[2]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(thirdTriangleVertices), thirdTriangleVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);

  // wireframe mode
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(blueShaderProgram);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(redShaderProgram);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(greenShaderProgram);
    glBindVertexArray(VAOs[2]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    glfwSwapBuffers(window);  // this will swap the color buffer used to render and show it as output to the screen
    glfwPollEvents();         // this checks if any events are triggered, updates the window state and execute callbacks
  }

  glDeleteVertexArrays(3, VAOs);
  glDeleteVertexArrays(3, VBOs);
  glDeleteProgram(blueShaderProgram);
  glDeleteProgram(redShaderProgram);
  glDeleteProgram(greenShaderProgram);
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
