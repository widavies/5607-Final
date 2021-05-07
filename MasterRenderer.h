#pragma once
#include "Renderer.h"

// Orchestrates entity rendering.
// This renderer will render entities in
// groups determined by the TexturedModel they share.
// This allows the MasterRenderer to optimize entity rendering
// by only binding the textures & geometry once for all entities that
// share them. 
class MasterRenderer {
public:
  MasterRenderer(DisplayManager& dm) {
    _shader = new StaticShader;
    _renderer = new Renderer(dm, *_shader);
  }
  ~MasterRenderer() {
    delete _renderer;
    delete _shader;
  }
  void render(Light& sun, Camera& camera);

  // Queues an entity for rendering. This should
  // be called once every frame if the entity needs to be renderer.
  void queueEntity(Entity& entity);
private:
  StaticShader* _shader;
  Renderer* _renderer;

  // An entity is an instance of a TexturedModel. This keeps track
  // of all the instances of particular TextureModel's
  std::map<TexturedModel*, std::vector<Entity*>> _entities;

};

