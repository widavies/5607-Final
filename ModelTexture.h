#pragma once
#include "glad/glad.h"
#include <SDL.h>
#include <SDL_opengl.h>

class ModelTexture {
public:

  ModelTexture(GLuint textureId) : _textureId(textureId) {}

  GLuint getTextureId() {
    return _textureId;
  }
private:
  GLuint _textureId;


};

