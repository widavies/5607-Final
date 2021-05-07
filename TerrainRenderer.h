#pragma once
#include "Terrain.h"
#include "DisplayManager.h"
#include "TexturedModel.h"
#include "TerrainShader.h"
#include <vector>


class TerrainRenderer {

public:
  TerrainRenderer(DisplayManager& dm, TerrainShader& shader, glm::mat4 projectMat)
    : _shader(shader) {
    _dm = &dm;

    // Create projection matrix
    shader.start();
    shader.setProjectionMat(projectMat);
    shader.stop();
  }

  void render(std::vector<Terrain*> terrains);
private:
  DisplayManager* _dm;
  TerrainShader& _shader;

  void prepareTerrain(Terrain* terrain);

  void unbindTextureModel();

  void prepareTerrainInstance(Terrain& terrain);
};