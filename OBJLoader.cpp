#include "OBJLoader.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Parses a str "1/2/3", inserting '1', '2', '3' into 'vals'
// as ints
void OBJLoader::parseFaceVertex(std::string str, int* vals) {
  std::string delimiter = "/";

  int index = 0;
  size_t pos = 0;
  while((pos = str.find(delimiter)) != std::string::npos) {
    vals[index] = std::stoi(str.substr(0, pos));
    str.erase(0, pos + delimiter.length());
    index++;
  }
  vals[index] = std::stoi(str);
}

void OBJLoader::parseFace(int* vals, std::vector<int>& indices, std::vector<glm::vec2>& textures,
  std::vector<glm::vec3> normals, float* _textures, float* _normals) {

  int currentVertexPointer = vals[0] - 1;
  indices.push_back(currentVertexPointer); 
  glm::vec2 currentTex = textures[vals[1] - 1];
  _textures[currentVertexPointer * 2] = currentTex.x;
  _textures[currentVertexPointer * 2 + 1] = 1 - currentTex.y;
  glm::vec3 currentNorm = normals[vals[2] - 1];
  _normals[currentVertexPointer * 3] = currentNorm.x;
  _normals[currentVertexPointer * 3 + 1] = currentNorm.y;
  _normals[currentVertexPointer * 3 + 2] = currentNorm.z;

}

RawModel OBJLoader::loadOBJ(std::string path, ModelLoader& loader) {

  std::ifstream file;
  file.open(path);

  if(!file) {
    std::cerr << "Failed to obj file: " << path << std::endl;
    exit(-1);
  }

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> textures;
  std::vector<glm::vec3> normals;
  std::vector<int> indices;

  float* _vertices = NULL;
  float* _textures = NULL;
  float* _normals = NULL;
  int* _indices = NULL;

  std::string op;

  float x, y, z;

  int index = 0;
  while(true) {
    file >> op;

    if(op == "v") {
      file >> x;
      file >> y;
      file >> z;

      vertices.push_back(glm::vec3(x, y, z));
    } else if(op == "vt") {
      file >> x;
      file >> y;

      textures.push_back(glm::vec2(x, y));
    } else if(op == "vn") {
      file >> x;
      file >> y;
      file >> z;

      normals.push_back(glm::vec3(x, y, z));
    } else if(op == "f") {
      _textures = new float[vertices.size() * 2];
      _normals = new float[vertices.size() * 3];
      break;
    }
    if (index % 100000 == 0) {
        std::cout << index << "\n";
    }
    index++;
  }

  std::string token;

  while(true) {
    int vals[3];

    file >> token;
    parseFaceVertex(token, vals);
    parseFace(vals, indices, textures, normals, _textures, _normals);

    file >> token;
    parseFaceVertex(token, vals);
    parseFace(vals, indices, textures, normals, _textures, _normals);

    file >> token;
    parseFaceVertex(token, vals);
    parseFace(vals, indices, textures, normals, _textures, _normals);

    if(!(file >> op)) {
      break;
    }

    if(op == "s") {
      file >> op;
      file >> op;
    }
  }

  _vertices = new float[vertices.size() * 3];
  _indices = new int[indices.size()];

  int vertexPointer = 0;

  for(std::vector<glm::vec3>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
    _vertices[vertexPointer++] = it->x;
    _vertices[vertexPointer++] = it->y;
    _vertices[vertexPointer++] = it->z;
  }

  for(int i = 0; i < indices.size(); i++) {
    _indices[i] = indices[i];
  }

  return loader.loadRaw(_vertices, vertices.size() * 3, _indices, indices.size(), _textures, vertices.size() * 2, _normals, vertices.size() * 3);
}

TexturedModel OBJLoader::loadTexturedOBJ(std::string objPath, std::string texturePath, ModelLoader& loader, float reflectivity, float shineDamper) {
  RawModel rawModel = OBJLoader::loadOBJ(objPath, loader);
  ModelTexture texture = loader.loadTexture(texturePath, shineDamper, reflectivity);

  return TexturedModel(rawModel, texture);
}
