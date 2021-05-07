#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Camera {
public:
  void move(float dx = 0.f, float dy = 0.f, float dz = 0.f) {
    _position.x += dx;
    _position.y += dy;
    _position.z += dz;
  }

  glm::vec3 getPosition() {
    return _position;
  }

  float getPitch() {
    return _pitch;
  }

  float getYaw() {
    return _yaw;
  }

  float getRoll() {
    return _roll;
  }


  glm::mat4 getViewMat() {
    glm::mat4 view(1.f);

    // Apply rotations
    view = glm::rotate(view, _pitch, glm::vec3(1.f, 0.f, 0.f));
    view = glm::rotate(view, _yaw, glm::vec3(0.f, 1.f, 0.f));
    view = glm::rotate(view, _roll, glm::vec3(0.f, 0.f, 1.f));

    // Apply the negative camera direction (world moves in opposite direction of the camera)
    view = glm::translate(view, glm::vec3(-_position.x, -_position.y, -_position.z));

    return view;
  }

private:
  glm::vec3 _position = glm::vec3(0.f, 15.f, 0.f);
  float _pitch = 1.5708f, _yaw = 0.f, _roll = 0.f;

};

