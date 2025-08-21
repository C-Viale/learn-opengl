#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "classes/shader.h"
#include "classes/texture.hpp"
#include "models/cube_model.hpp"
#include "stb_image.h"
#include "util.h"

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, GLfloat deltaTime);
GLFWwindow *initWindow();

const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

float fov = 45.0f;

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

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  projection = glm::perspective(glm::radians(fov), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);

  defaultShader.use();
  defaultShader.setMat4("view", view);
  defaultShader.setMat4("model", model);
  defaultShader.setMat4("projection", projection);

  glEnable(GL_DEPTH_TEST);

  // render loop
  GLfloat deltaTime = 0.0f;
  GLfloat lastFrame = 0.0f;

  while (!glfwWindowShouldClose(window)) {
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window, deltaTime);

    defaultShader.use();

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(fov), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);

    defaultShader.setMat4("view", view);
    defaultShader.setMat4("projection", projection);

    for (unsigned int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);

      float angle = 20.0f * i;

      if (i % 3 == 0) {
        angle = currentFrame * 25.0f;
      }

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

void processInput(GLFWwindow *window, GLfloat deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    fov = clip(fov + 10.0f * deltaTime, 45, 100.0f);
  }

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    fov = clip(fov - 10.0f * deltaTime, 45, 100.0f);
  }
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