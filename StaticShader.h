#pragma once
#include "ShaderProgram.h"
#include <string>

// https://stackoverflow.com/a/1372858/4779937
class StaticShader : public ShaderProgram {

public:
  StaticShader() 
    : ShaderProgram("vertexShader.txt", "fragmentShader.txt", 
      new std::string[]{"position", "textureCoords"}, 2) {
    
    
  }
};

