#pragma once
#include "RawModel.h"
#include "DisplayManager.h"
#include "TexturedModel.h"
#include "Entity.h"
#include "StaticShader.h"
#include <vector>
#include <map>

// Basic utility rendering functions
class EntityRenderer {

public:
  EntityRenderer(DisplayManager& dm, StaticShader& shader, glm::mat4 projectMat) 
    : _shader(shader) {
    _dm = &dm;

    // Create projection matrix
    shader.start();
    shader.setProjectionMat(projectMat);
    shader.stop();
  }

  void render(std::map<TexturedModel*, std::vector<Entity*>>& entities);
private:
  DisplayManager* _dm;
  StaticShader& _shader;
  
  void prepareTextureModel(TexturedModel& model);

  void unbindTextureModel();

  void prepareInstance(Entity& entity);
};