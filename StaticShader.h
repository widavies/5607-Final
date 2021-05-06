#pragma once
#include "ShaderProgram.h"
#include <string>

// https://stackoverflow.com/a/1372858/4779937
class StaticShader : public ShaderProgram {

public:
  StaticShader() 
    : ShaderProgram("vertexShader.txt", "fragmentShader.txt") {}

protected:
  void bindAttributes();

};

