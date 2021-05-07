#include "ModelLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

RawModel ModelLoader::loadRaw(float* positions, int positionsSize, int * indices, int indicesSize, float * textureCoords, int texturesSize, float * normals, int normalsSize) {
  GLuint id = createVAO();
  bindIndexBuffer(indices, indicesSize);
  saveDataAttr(0, 3, positions, positionsSize);
  saveDataAttr(1, 2, textureCoords, texturesSize);
  saveDataAttr(2, 3, normals, normalsSize);
  
  unbindVAO();

  return RawModel(id, indicesSize);
}

// https://learnopengl.com/Getting-started/Textures
ModelTexture ModelLoader::loadTexture(std::string path) {
  GLuint id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  int width, height, channels;
  unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  if(data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture: " << path << std::endl;
    exit(-1);
  }
  stbi_image_free(data);

  _textures.push_back(id);

  return ModelTexture(id);
}

void ModelLoader::close() {

  glDeleteVertexArrays(_vaos.size(), &_vaos[0]);
  glDeleteBuffers(_vbos.size(), &_vbos[0]);
  glDeleteTextures(_textures.size(), &_textures[0]);

}

GLuint ModelLoader::createVAO() {
  GLuint id;

  glGenVertexArrays(1, &id);
  glBindVertexArray(id);

  _vaos.push_back(id);

  return id;
}

void ModelLoader::saveDataAttr(int attrNumber, int unitSize, float* data, int size) {
  GLuint id;

  // Creates a VBO
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);

  // Saves data to the VBO
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);

  // Inserts the VBO into the VAO
  // Takes index, (size of items), type, normalized, stride, offset
  glVertexAttribPointer(attrNumber, unitSize, GL_FLOAT, GL_FALSE, 0, 0);

  // Unbinds VBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  _vbos.push_back(id);

}

void ModelLoader::bindIndexBuffer(int* indices, int size) {
  // Create a VBO for holding the index buffer
  GLuint id;

  glGenBuffers(1, &id);

  // Binding works a bit differently here, binding an index buffer
  // binds it to the active VAO. Unbinding it removes it.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(int), indices, GL_STATIC_DRAW);

  _vbos.push_back(id);
}

void ModelLoader::unbindVAO() {
  glBindVertexArray(0);
}
