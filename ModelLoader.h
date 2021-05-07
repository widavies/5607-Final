#pragma once
#include "RawModel.h"
#include "ModelTexture.h"
#include <vector>
#include <string>

class ModelLoader {

public:

  // Loads a model from raw vertex positions.
  RawModel loadRaw(float* positions, int positionsSize, int* indices, int indicesSize, float* textureCoords, int texturesSize, float* normals, int normalsSize);

  ModelTexture loadTexture(std::string path);

  void close();

private:
  // Tracks vaos so they can be free'd when the program exits
  std::vector<GLuint> _vaos;

  // Tracks vbos so they can be free'd when the program exits
  std::vector<GLuint> _vbos;

  std::vector<GLuint> _textures;

  // Creates and binds a VAO - a data structure that describes
  // an object
  GLuint createVAO();

  // Saves an attribute to the currently bound VAO
  void saveDataAttr(int attrNumber, int unitSize, float* data, int size);

  // For performance, vertices shared by multiple triangles are merged
  // into a single vertex. An index buffer describes how vertices should
  // be connected to form triangles. An index buffer doesn't describe _actual_
  // data (lighting, positioning, textures, etc) and is thus not an attribute. 
  // OpenGL will bind it to the currently active VAO
  void bindIndexBuffer(int * indices, int size);

  // Unbinds the active VAO
  void unbindVAO();
};

