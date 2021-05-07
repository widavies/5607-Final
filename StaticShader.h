#pragma once
#include "ShaderProgram.h"
#include <string>
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include "Camera.h"
#include "Light.h"

// https://stackoverflow.com/a/1372858/4779937
class StaticShader : public ShaderProgram {

public:
  StaticShader() 
    : ShaderProgram("vertexShader.txt", "fragmentShader.txt", 
      new std::string[]{"position", "textureCoords", "normal"}, 3) {

    _locTransformMat = getUniformLocation("transformationMatrix");
    _locProjectionMat = getUniformLocation("projectionMatrix");
    _locViewMat = getUniformLocation("viewMatrix");
    _locLightPosition = getUniformLocation("lightPosition");
    _locLightColor = getUniformLocation("lightColor");
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
  void setLight(Light& light) {
    setUniformVec3(_locLightPosition, light.getPosition());
    setUniformVec3(_locLightColor, light.getColor());
  }
private:
  GLuint _locTransformMat;
  GLuint _locProjectionMat;
  GLuint _locViewMat;
  GLuint _locLightPosition;
  GLuint _locLightColor;

};

