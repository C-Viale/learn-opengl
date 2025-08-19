#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
 public:
  unsigned int ID;

  // constructor
  Shader(const char* vertexPath, const char* fragmentPath);

  // activate shader
  void use();

  // uniform utility functions
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
  void setMat4(const std::string& name, glm::mat4 matrix) const;
};

unsigned int buildShaderProgram();
#endif
