#pragma once
#include <string>
#include "ModelLoader.h"
#include "RawModel.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class OBJLoader {

public:
  static RawModel loadOBJ(std::string path, ModelLoader& loader);

private:
  static void parseFaceVertex(std::string str, int* vals);

  static void parseFace(int* vals, std::vector<int>& indices, std::vector<glm::vec2>& textures, 
    std::vector<glm::vec3> normals, float * _textures, float * _normals);

  

};

