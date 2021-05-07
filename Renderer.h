#pragma once
#include "RawModel.h"
#include "DisplayManager.h"
#include "TexturedModel.h"
#include "Entity.h"
#include "StaticShader.h"
#include <vector>
#include <map>

// Basic utility rendering functions
class Renderer {

public:
  const float FOV = 70;
  const float NEAR_PLANE = 0.01f;
  const float FAR_PLANE = 1000.f;

  Renderer(DisplayManager& dm, StaticShader& shader) : _shader(shader) {
    _dm = &dm;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Create projection matrix
    glm::mat4 proj = glm::perspective(glm::radians(FOV), _dm->aspect, NEAR_PLANE, FAR_PLANE);
    shader.start();
    shader.setProjectionMat(proj);
    shader.stop();
  }

  void prepare();

  void render(std::map<TexturedModel*, std::vector<Entity*>>& entities);

  // Flushes all rendering buffers to the screen
  void flush();
private:
  DisplayManager* _dm;
  StaticShader& _shader;

  void prepareTextureModel(TexturedModel& model);

  void unbindTextureModel();

  void prepareInstance(Entity& entity);
};