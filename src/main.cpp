#include <glad/glad.h>
//
#include <GLFW/glfw3.h>

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "shader.h"
#include "stb_image.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, GLfloat deltaTime);

float clip(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}

const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

float mixValue = 0.2f;

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

  Shader defaultShader("/Users/caio/Development/opengl/src/shaders/default/vertex.glsl", "/Users/caio/Development/opengl/src/shaders/default/fragment.glsl");
  // Shader testShader("/Users/caio/Development/opengl/src/shaders/test/vertex.glsl", "/Users/caio/Development/opengl/src/shaders/test/fragment.glsl");

  float vertices[] = {
      // positions          // colors           // texture coords
      0.5f, 0.5f, 0.0f, /**/ 1.0f, 0.0f, 0.0f, /**/ 1.0f, 1.0f,    // top right
      0.5f, -0.5f, 0.0f, /**/ 0.0f, 1.0f, 0.0f, /**/ 1.0, 0.0f,    // bottom right
      -0.5f, -0.5f, 0.0f, /**/ 0.0f, 0.0f, 1.0f, /**/ 0.0f, 0.0f,  // bottom left
      -0.5f, 0.5f, 0.0f, /**/ 1.0f, 1.0f, 0.0f, /**/ 0.0f, 1.0f    // top left
  };

  unsigned int indices[] = {
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };
  unsigned int VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  /*
     The Vertex Array Object (VAO) is bound like the Vertex Buffer Object (VBO).
     Any vertex attribute calls after the bound will be stored inside the VAO.
     Because of this, whe bind the VAO before the VBO
  */
  glBindVertexArray(VAO);                                                     // 1. bind VAO first
  glBindBuffer(GL_ARRAY_BUFFER, VBO);                                         // 2. bind VBO to GL_ARRAY_BUFFER
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // 3. set vertex data to the buffer

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);  // 4. configure vertex attributes
  glEnableVertexAttribArray(0);                                                  // 5. enable vertex attribute at location 0
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannels;
  unsigned char* data = stbi_load("/Users/caio/Development/opengl/src/assets/container.png", &width, &height, &nrChannels, 0);

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                     // texture wrapping in X axis
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);                     // texture wrapping in Y axis
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);  // nearest mipmap level, linear filtering on mipmap level
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);                 // linear filtering when magnifiying

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

  } else {
    std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD" << std::endl;
  }

  stbi_image_free(data);

  unsigned char* data2 = stbi_load("/Users/caio/Development/opengl/src/assets/awesome.png", &width, &height, &nrChannels, 0);
  unsigned int texture2;
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (data2) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD" << std::endl;
  }

  stbi_image_free(data2);

  defaultShader.use();
  defaultShader.setInt("texture1", 0);
  defaultShader.setInt("texture2", 1);
  defaultShader.setFloat("texOpacity", mixValue);

  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);

  // wireframe mode
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // render loop

  GLfloat deltaTime = 0.0f;
  GLfloat lastFrame = 0.0f;

  while (!glfwWindowShouldClose(window)) {
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window, deltaTime);

    defaultShader.use();
    defaultShader.setFloat("texOpacity", mixValue);

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    glfwSwapBuffers(window);  // this will swap the color buffer used to render and show it as output to the screen
    glfwPollEvents();         // this checks if any events are triggered, updates the window state and execute callbacks
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteVertexArrays(1, &VBO);
  glDeleteProgram(defaultShader.ID);
  glfwTerminate();
  return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, GLfloat deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    mixValue = clip(mixValue + 0.2f * deltaTime, 0, 1.0f);
  }

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    mixValue = clip(mixValue - 0.2f * deltaTime, 0, 1.0f);
  }
}
