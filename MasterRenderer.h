#pragma once
#include "Renderer.h"

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
  void queueEntity(Entity& entity);
private:
  StaticShader* _shader;
  Renderer* _renderer;

  // An entity is an instance of a TexturedModel. This keeps track
  // of all the instances of particular TextureModel's
  std::map<TexturedModel*, std::vector<Entity*>> _entities;

};

