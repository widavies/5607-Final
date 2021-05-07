#pragma once
#include "EntityRenderer.h"
#include "TerrainRenderer.h"

// Orchestrates entity rendering.
// This renderer will render entities in
// groups determined by the TexturedModel they share.
// This allows the MasterRenderer to optimize entity rendering
// by only binding the textures & geometry once for all entities that
// share them. 
class MasterRenderer {
public:
  const float FOV = 70;
  const float NEAR_PLANE = 0.01f;
  const float FAR_PLANE = 1000.f;

  MasterRenderer(DisplayManager& dm) : _dm(dm) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Create projection matrix
    glm::mat4 proj = glm::perspective(glm::radians(FOV), dm.aspect, NEAR_PLANE, FAR_PLANE);

    _entityShader = new StaticShader;
    _entityRenderer = new EntityRenderer(dm, *_entityShader, proj);

    _terrainShader = new TerrainShader;
    _terrainRenderer = new TerrainRenderer(dm, *_terrainShader, proj);

  }
  ~MasterRenderer() {
    delete _entityRenderer;
    delete _entityShader;
    delete _terrainRenderer;
    delete _terrainShader;
  }
  void prepare();

  void render(Light& sun, Camera& camera);

  // Queues an entity for rendering. This should
  // be called once every frame if the entity needs to be renderer.
  void queueEntity(Entity& entity);

  void queueTerrain(Terrain& terrain);
private:
  DisplayManager& _dm;
  StaticShader* _entityShader;
  TerrainShader* _terrainShader;
  EntityRenderer* _entityRenderer;
  TerrainRenderer* _terrainRenderer;

  // An entity is an instance of a TexturedModel. This keeps track
  // of all the instances of particular TextureModel's
  std::map<TexturedModel*, std::vector<Entity*>> _entities;

  std::vector<Terrain*> _terrains;

};

