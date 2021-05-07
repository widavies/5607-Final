#pragma once
#include "glad/glad.h"
#include <SDL.h>
#include <SDL_opengl.h>

// Stores the geometry of an object
class RawModel {

public:
  RawModel() {
    _id = 0;
    _vertexCount = 0;
  }
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

