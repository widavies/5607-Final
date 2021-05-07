#pragma once
#include "glad/glad.h"
#include <SDL.h>
#include <SDL_opengl.h>

class ModelTexture {
public:

  ModelTexture(GLuint textureId, float shineDamper, float reflectivity) : _textureId(textureId), 
    _shineDamper(shineDamper), _reflectivity(reflectivity) {}

  GLuint getTextureId() {
    return _textureId;
  }

  float getShineDamper() {
    return _shineDamper;
  }

  float getReflectivity() {
    return _reflectivity;
  }

private:
  GLuint _textureId;
  float _shineDamper = 1.f;
  float _reflectivity = 0.f;

};

