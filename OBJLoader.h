#pragma once
#include <string>
#include "ModelLoader.h"
#include "TexturedModel.h"
#include "RawModel.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Converts the '.obj' file format into OpenGL ready vertex/normal/index array for rendering
class OBJLoader {

public:
  // path - the location of the '.obj' file
  static RawModel loadOBJ(std::string path, ModelLoader& loader);

  static TexturedModel loadTexturedOBJ(std::string objPath, std::string texturePath, ModelLoader& loader, float reflectivity = 0.f, float shineDamper = 1.f);

private:
  // Takes a str of the form '123/456/689', separates it by '/',
  // and saves the integers to the array 'vals'
  static void parseFaceVertex(std::string str, int* vals);

  static void parseFace(int* vals, std::vector<int>& indices, std::vector<glm::vec2>& textures, 
    std::vector<glm::vec3> normals, float * _textures, float * _normals);

};

