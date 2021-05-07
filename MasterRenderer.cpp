#include "MasterRenderer.h"

void MasterRenderer::render(Light& sun, Camera& camera) {

  prepare();
  //_entityShader->start();
  //_entityShader->setLight(sun);
  //_entityShader->setViewMat(camera);
  //_entityRenderer->render(_entities);
  //_entityShader->stop();

  _terrainShader->start();
  _terrainShader->setLight(sun);
  _terrainShader->setViewMat(camera);
  _terrainRenderer->render(_terrains);
  _terrainShader->stop();

  _terrains.clear();
  _entities.clear();

  _dm.Swap();
}

void MasterRenderer::queueEntity(Entity& entity) {

  TexturedModel* model = entity.getModel();

  std::map<TexturedModel*, std::vector<Entity*>>::iterator it = _entities.find(model);

  if(it != _entities.end()) {
    it->second.push_back(&entity);
  } else {
    std::vector<Entity*> batch;
    batch.push_back(&entity);
    _entities.insert(std::pair<TexturedModel*, std::vector<Entity*>>(model, batch));
  }

}

void MasterRenderer::queueTerrain(Terrain& terrain) {

  _terrains.push_back(&terrain);

}

void MasterRenderer::prepare() {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.f, 0.f, 0.f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
