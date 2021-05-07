#pragma once
#include "ShaderProgram.h"
#include <string>
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include "Camera.h"
#include "Light.h"

// https://stackoverflow.com/a/1372858/4779937
// An implementation of ShaderProgram. 
// Basically just loads the shader and binds all the uniform variables from GLSL into C++
// code so they can be easily set
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
    _locShineDamper = getUniformLocation("shineDamper");
    _locReflectivity = getUniformLocation("reflectivity");
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
  void setShine(float damper, float reflectivity) {
    setUniformFloat(_locShineDamper, damper);
    setUniformFloat(_locReflectivity, reflectivity);
  }
private:
  GLuint _locTransformMat;
  GLuint _locProjectionMat;
  GLuint _locViewMat;
  GLuint _locLightPosition;
  GLuint _locLightColor;
  GLuint _locShineDamper;
  GLuint _locReflectivity;

};

