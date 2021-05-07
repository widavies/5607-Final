#pragma once
#include "ShaderProgram.h"
#include <string>
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include "Camera.h"

// https://stackoverflow.com/a/1372858/4779937
class StaticShader : public ShaderProgram {

public:
  StaticShader() 
    : ShaderProgram("vertexShader.txt", "fragmentShader.txt", 
      new std::string[]{"position", "textureCoords"}, 2) {

    _locTransformMat = getUniformLocation("transformationMatrix");
    _locProjectionMat = getUniformLocation("projectionMatrix");
    _locViewMat = getUniformLocation("viewMatrix");
  }
  void setTransformMat(glm::mat4 transform) {
    setUniformMat4(_locTransformMat, transform);
  }
  void setProjectionMat(glm::mat4 projection) {
    setUniformMat4(_locProjectionMat, projection);
  }
  void setViewMat(Camera& camera) {
    setUniformMat4(_locViewMat, camera.getViewMat());
  }
private:
  GLuint _locTransformMat;
  GLuint _locProjectionMat;
  GLuint _locViewMat;

};

