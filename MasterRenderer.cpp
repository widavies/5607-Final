#include "MasterRenderer.h"

void MasterRenderer::render(Light& sun, Camera& camera) {

  _renderer->prepare();
  _shader->start();
  _shader->setLight(sun);
  _shader->setViewMat(camera);
  _renderer->render(_entities);
  _shader->stop();
  _entities.clear();

  _renderer->flush();
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
