#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <cassert>

ShaderProgram::ShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath, std::string* attrs, int attrCount) {

  _vertexShaderId = loadShader(vertexShaderPath, GL_VERTEX_SHADER);
  _fragmentShaderId = loadShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

  _programId = glCreateProgram();
  glAttachShader(_programId, _vertexShaderId);
  glAttachShader(_programId, _fragmentShaderId);

  // The out color in the fragment shader that will be used for each pixel
  //glBindFragDataLocation(_programId, 0, "outColor");

  for(int i = 0; i < attrCount; i++) {
    bindAttribute(i, attrs[i]);
  }

  glLinkProgram(_programId);
  glValidateProgram(_programId);
}

ShaderProgram::~ShaderProgram() {
  stop();
  glDetachShader(_programId, _vertexShaderId);
  glDetachShader(_programId, _fragmentShaderId);
  glDeleteShader(_vertexShaderId);
  glDeleteShader(_fragmentShaderId);
  glDeleteProgram(_programId);
}

void ShaderProgram::start() {
  glUseProgram(_programId);
}

void ShaderProgram::stop() {
  glUseProgram(0);
}

void ShaderProgram::bindAttribute(int attribute, std::string name) {

  glBindAttribLocation(_programId, attribute, name.c_str());

}

GLuint ShaderProgram::loadShader(std::string path, int type) {
  assert(type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER);

  // Load the shader program
  std::ifstream file;
  file.open(path);

  if(!file) {
    std::cerr << "Failed to load shader program: " << path << std::endl;
    exit(-1);
  }

  std::string source;

  std::string line;

  while(getline(file, line)) {
    source = source + line + "\n";
  }

  const GLchar* csource = source.c_str();

  // Compile the shader program
  GLuint id = glCreateShader(type);
  glShaderSource(id, 1, &csource, NULL);
  glCompileShader(id);

  // Check to see if the shader program compiled
  GLint status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if(!status) {
    char buffer[512];
    glGetShaderInfoLog(id, 512, NULL, buffer);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
      "Compilation Error",
      "Failed to Compile: Check Consol Output.",
      NULL);
    printf("Vertex Shader Compile Failed. Info:\n\n%s\n%s\n", path.c_str(), buffer);
  } else {
    std::cout << "Shader " << path << " compiled successfully" << std::endl;
  }

  return id;
}
