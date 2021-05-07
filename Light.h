#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Light {
public:
  Light(glm::vec3 position, glm::vec3 color) : _position(position), _color(color) {

  }

  glm::vec3 getPosition() {
    return _position;
  }

  glm::vec3 getColor() {
    return _color;
  }

private:
  glm::vec3 _position;
  glm::vec3 _color;

};

