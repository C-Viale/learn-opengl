#include <glad/glad.h>
//
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader.h"

unsigned int compileVertexShader() {
  const char* vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main() {\n"
      "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
      "}";

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);  // create shader pointer
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);    // bind source to shader
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::VERTEX::SHADER::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  return vertexShader;
}

unsigned int compileFragmentShader(SHADER_COLOR color) {
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  if (color == SHADER_COLOR::red) {
    const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "  FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
        "}";
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  } else if (color == SHADER_COLOR::blue) {
    const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "  FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
        "}";
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  } else {
    const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "  FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
        "}";
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  }

  glCompileShader(fragmentShader);

  int success;
  char infoLog[512];
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  return fragmentShader;
}

unsigned int buildColorShaderProgram(SHADER_COLOR color) {
  unsigned int vertexShader = compileVertexShader();
  unsigned int fragmentShader = compileFragmentShader(color);

  unsigned int shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}