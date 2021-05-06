#include "Renderer.h"
#include "RawModel.h"


void Renderer::prepare() {

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

void Renderer::flush() {
  _dm->Swap();
}
