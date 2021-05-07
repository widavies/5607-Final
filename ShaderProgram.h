#pragma once

#include <string>

#include "glad/glad.h" 
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

// A generic shader program.
class ShaderProgram {

public:
  // Loads and compiles the shader
  // attrs are the inputs to the vertex shader
  ShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath, std::string * attrs, int attrCount);
  ~ShaderProgram();

  void start();

  void stop();

protected:
  int getUniformLocation(std::string name) {
    return glGetUniformLocation(_programId, name.c_str());
  }

  void setUniformFloat(GLuint location, float value) {
    glUniform1f(location, value);
  }

  void setUniformVec3(GLuint location, glm::vec3 value) {
    glUniform3f(location, value.x, value.y, value.z);
  }

  void setUniformBool(GLuint location, bool value) {
    glUniform1f(location, value ? 1 : 0);
  }

  void setUniformMat4(GLuint location, glm::mat4 value) {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
  }

private:
  void bindAttribute(int attribute, std::string name);

  static GLuint loadShader(std::string path, int type);

  GLuint _programId;
  GLuint _vertexShaderId;
  GLuint _fragmentShaderId;
};

