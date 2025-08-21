#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../stb_image.h"

class Texture {
public:
  unsigned int ID;
  int width, height, nrChannels;

  Texture(const char *filename, int colorScheme) {
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // texture wrapping in X axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // texture wrapping in Y axis

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); // nearest mipmap level, linear filtering on mipmap level
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);                // linear filtering when magnifiying

    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, colorScheme, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

    } else {
      std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD" << std::endl;
    }

    stbi_image_free(data);
  }
};

#endif