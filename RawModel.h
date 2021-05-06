#pragma once
#include "glad/glad.h"
#include <SDL.h>
#include <SDL_opengl.h>
class RawModel {

public:
  RawModel(GLuint id, int vertexCount) : _id(id), _vertexCount(vertexCount) {}

  int getVertexCount() {
    return _vertexCount;
  }

  GLuint getVaoId() {
    return _id;
  }

private:
  // VAO id
  GLuint _id;
  int _vertexCount;

};

