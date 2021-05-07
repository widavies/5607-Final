#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "DisplayManager.h"

#include <cmath>

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

 /* float toRadians(double angle_in_degrees) {
      return angle_in_degrees * (3.1415 / 180.0);
  }*/

  void calculatePitch(int yDelta) {
      _pitch -= yDelta * sensitivity;
  }

  void calculateAngleAroundPlayer(int xDelta) {
      angleAroundPlayer -= xDelta * sensitivity;

  }

  float calculateHorizontalDistance() {
      return (float)(distanceFromPlayer * cos(_pitch));
  }

  float calculateVerticalDistance() {
      return (float)(distanceFromPlayer * sin(_pitch));
  }


  void calculateCameraPosition(float horizDistance, float verticDistance, float x_pos, float y_pos, float z_pos, float y_rot) {
    _position.y = y_pos + verticDistance;

    float theta = y_rot + angleAroundPlayer;
    float offsetX = horizDistance * sin(theta);
    float offsetZ = horizDistance * cos(theta);
    _position.x = x_pos + offsetX; 
    _position.z = z_pos - offsetZ;

    _yaw = (3.1415 - y_rot + angleAroundPlayer);
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
  glm::vec3 _position = glm::vec3(0.f, 0.f, 0.f);
  float _pitch = 1.5708f, _yaw = 0.f, _roll = 0.f;
  float angleAroundPlayer = 0;
  float distanceFromPlayer = 15;
  float sensitivity = .02;

};

