#pragma once

#include <string>

#include "glad/glad.h" 
#include <SDL.h>
#include <SDL_opengl.h>

class ShaderProgram {

public:
  // Loads and compiles the shader
  ShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath);
  ~ShaderProgram();

  void start();

  void stop();

protected:

  virtual void bindAttributes() = 0;
  void bindAttribute(int attribute, std::string name);

private:
  static GLuint loadShader(std::string path, int type);

  GLuint _programId;
  GLuint _vertexShaderId;
  GLuint _fragmentShaderId;
};

