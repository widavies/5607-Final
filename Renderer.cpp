#include "Renderer.h"
#include "RawModel.h"

void Renderer::prepare() {
  glEnable(GL_DEPTH_TEST);
  glClearColor(1.f, 1.f, 1.f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::flush() {
  _dm->Swap();
}

void Renderer::render(std::map<TexturedModel*, std::vector<Entity*>>& entities) {

  for(std::map <TexturedModel*, std::vector<Entity*>>::iterator it = entities.begin(); it != entities.end(); ++it) {
    TexturedModel* model = it->first;

    prepareTextureModel(*model);

    for(int i = 0; i < it->second.size(); i++) {
      Entity* entity = it->second[i];

      prepareInstance(*entity);

      glDrawElements(GL_TRIANGLES, model->getModel().getVertexCount(), GL_UNSIGNED_INT, 0);
    }
    unbindTextureModel();
  }

}


void Renderer::prepareTextureModel(TexturedModel& model) {
  RawModel& rawModel = model.getModel();

  glBindVertexArray(rawModel.getVaoId());
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  ModelTexture& texture = model.getTexture();
  _shader.setShine(texture.getShineDamper(), texture.getReflectivity());

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.getTextureId());

}

void Renderer::unbindTextureModel() {
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);

  glBindVertexArray(0);
}

void Renderer::prepareInstance(Entity& entity) {
  glm::mat4 transformMat = entity.getTransformMat();

  _shader.setTransformMat(transformMat);
}
