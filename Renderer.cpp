#include "Renderer.h"
#include "RawModel.h"

void Renderer::prepare() {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.f, 0.5f, 0.f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(RawModel& model) {
  glBindVertexArray(model.getVaoId());
  glEnableVertexAttribArray(0);
  //glDrawArrays(GL_TRIANGLES, 0, model.getVertexCount());
  glDrawElements(GL_TRIANGLES, model.getVertexCount(), GL_UNSIGNED_INT, 0);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

void Renderer::render(Entity& entity, StaticShader& shader) {
  RawModel rawModel = entity.getModel();

  glBindVertexArray(rawModel.getVaoId());
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glm::mat4 transformMat = entity.getTransformMat();

  shader.setTransformMat(transformMat);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, entity.getTexture().getTextureId());
  //glDrawArrays(GL_TRIANGLES, 0, model.getVertexCount());
  glDrawElements(GL_TRIANGLES, rawModel.getVertexCount(), GL_UNSIGNED_INT, 0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glBindVertexArray(0);
}

void Renderer::flush() {
  _dm->Swap();
}
