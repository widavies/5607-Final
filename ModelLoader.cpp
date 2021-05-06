#include "ModelLoader.h"

RawModel ModelLoader::loadRaw(float* positions, int positionsSize, int * indices, int indicesSize) {
  GLuint id = createVAO();
  bindIndexBuffer(indices, indicesSize);
  saveDataAttr(0, positions, positionsSize);
  unbindVAO();

  return RawModel(id, indicesSize);
}

void ModelLoader::close() {

  glDeleteVertexArrays(_vaos.size(), &_vaos[0]);
  glDeleteBuffers(_vbos.size(), &_vbos[0]);

}

GLuint ModelLoader::createVAO() {
  GLuint id;

  glGenVertexArrays(1, &id);
  glBindVertexArray(id);

  _vaos.push_back(id);

  return id;
}

void ModelLoader::saveDataAttr(int attrNumber, float* data, int size) {
  GLuint id;

  // Creates a VBO
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);

  // Saves data to the VBO
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);

  // Inserts the VBO into the VAO
  // Takes index, (size of items), type, normalized, stride, offset
  glVertexAttribPointer(attrNumber, 3, GL_FLOAT, GL_FALSE, 0, 0);

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
