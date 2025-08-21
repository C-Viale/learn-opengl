#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>

#include "classes/camera.hpp"
#include "classes/shader.h"
#include "classes/texture.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "models/cube_model.hpp"
#include "stb_image.h"
#include "util.h"

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
GLFWwindow *initWindow();

const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = false;

Camera camera(glm::vec3(0, 0, 3.0f));

int main() {
  GLFWwindow *window = initWindow();

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

  Shader defaultShader("/Users/caio/Development/opengl/src/shaders/default/vertex.glsl", "/Users/caio/Development/opengl/src/shaders/default/fragment.glsl");
  Texture woodTexture("/Users/caio/Development/opengl/src/assets/container.png", GL_RGB);
  Texture awesomeTexture("/Users/caio/Development/opengl/src/assets/awesome.png", GL_RGBA);

  CubeModel cube(&defaultShader, woodTexture.ID, awesomeTexture.ID);

  // world space positions of our cubes
  glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3(2.0f, 5.0f, -15.0f), glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                               glm::vec3(2.4f, -0.4f, -3.5f), glm::vec3(-1.7f, 3.0f, -7.5f), glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                               glm::vec3(1.5f, 0.2f, -1.5f),  glm::vec3(-1.3f, 1.0f, -1.5f)};

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);
  glEnable(GL_DEPTH_TEST);

  const float radius = 10.0f;

  // render loop
  while (!glfwWindowShouldClose(window)) {
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    defaultShader.use();

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(camera.Zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);

    defaultShader.setMat4("view", view);
    defaultShader.setMat4("projection", projection);

    for (unsigned int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);

      float angle = 20.0f * i;

      // if (i % 3 == 0) {
      //   angle = currentFrame * 25.0f;
      // }

      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      defaultShader.setMat4("model", model);
      cube.render();
    }

    glfwSwapBuffers(window); // this will swap the color buffer used to render and show it as output to the screen
    glfwPollEvents();        // this checks if any events are triggered, updates the window state and execute callbacks
  }

  cube.destroy();
  glDeleteProgram(defaultShader.ID);
  glfwTerminate();
  return 0;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }

float lastX = 400, lastY = 300;

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top;
  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  //
  camera.ProcessMouseScroll((float)yoffset);
}

GLFWwindow *initWindow() {
  // initialize GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // initialize window
  GLFWwindow *window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "HELLO WORLD", NULL, NULL);
  return window;
}