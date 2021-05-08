#pragma once
#include "ShaderProgram.h"
#include <string>
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include "Camera.h"
#include "Light.h"

class SkyboxShader : public ShaderProgram {

public:
  SkyboxShader()
    : ShaderProgram("skyboxVertexShader.txt", "skyboxFragmentShader.txt",
      new std::string[]{ "position"}, 1) {

    _locProjectionMat = getUniformLocation("projectionMatrix");
    _locViewMat = getUniformLocation("viewMatrix");
  }

  void setProjectionMat(glm::mat4 projection) {
    setUniformMat4(_locProjectionMat, projection);
  }
  void setViewMat(Camera& camera) {
    // Take out the translation
    glm::mat4 view = camera.getViewMat();

    view[3].x = 0;
    view[3].y = 0;
    view[3].z = 0;

    
    setUniformMat4(_locViewMat, view);
  }
private:
  GLuint _locProjectionMat;
  GLuint _locViewMat;

};



