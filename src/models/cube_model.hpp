#ifndef CUBEMODEL_H
#define CUBEMODEL_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "../classes/shader.h"

class CubeModel {
public:
  float vertices[180] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,  0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,
      0.5f,  -0.5f, 1.0f,  1.0f, -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,
      0.5f,  0.5f,  1.0f,  1.0f, -0.5f, 0.5f,  0.5f,  0.0f,  1.0f, -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f,
      -0.5f, -0.5f, 0.0f,  1.0f, -0.5f, -0.5f, 0.5f,  0.0f,  0.0f, -0.5f, 0.5f,  0.5f,  1.0f,  0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,
      -0.5f, -0.5f, 0.0f,  1.0f, 0.5f,  -0.5f, 0.5f,  0.0f,  0.0f, 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,  0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,
      -0.5f, 0.5f,  1.0f,  0.0f, -0.5f, -0.5f, 0.5f,  0.0f,  0.0f, -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
      0.5f,  0.5f,  1.0f,  0.0f, -0.5f, 0.5f,  0.5f,  0.0f,  0.0f, -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f
      //
  };

  unsigned int VAO;
  unsigned int VBO;

  Shader *shader;
  unsigned int texture1;
  unsigned int texture2;

  CubeModel(Shader *shader, unsigned int t1, unsigned int t2) : shader(shader), texture1(t1), texture2(t2) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    /*
     The Vertex Array Object (VAO) is bound like the Vertex Buffer Object (VBO).
     Any vertex attribute calls after the bound will be stored inside the VAO.
     Because of this, whe bind the VAO before the VBO
  */
    glBindVertexArray(VAO);                                                        // 1. bind VAO first
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                            // 2. bind VBO to GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);     // 3. set vertex data to the buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0); // 4. configure vertex attributes
    glEnableVertexAttribArray(0);                                                  // 5. enable vertex attribute at location 0
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
  }

  void render() {
    shader->use();
    shader->setInt("texture1", 0);
    shader->setInt("texture2", 1);

    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

  void destroy() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VBO);
  }
};

#endif