#include "TerrainRenderer.h"
#include <cassert>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void TerrainRenderer::render(std::vector<Terrain*> terrains) {

  for(int i = 0; i < terrains.size(); i++) {
    Terrain* terrain = terrains[i];

    prepareTerrain(terrain);

    prepareTerrainInstance(*terrain);
    glDrawElements(GL_TRIANGLES, terrain->getModel().getVertexCount(), GL_UNSIGNED_INT, 0);
    unbindTextureModel();
  }

}

void TerrainRenderer::prepareTerrain(Terrain* terrain) {
  RawModel& rawModel = terrain->getModel();

  assert(rawModel.getVaoId() != 0);

  glBindVertexArray(rawModel.getVaoId());
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  ModelTexture* texture = terrain->getTexture();
  _shader.setShine(texture->getShineDamper(), texture->getReflectivity());

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture->getTextureId());

}

void TerrainRenderer::unbindTextureModel() {
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);

  glBindVertexArray(0);
}

void TerrainRenderer::prepareTerrainInstance(Terrain& terrain) {
  glm::mat4 mat(1.f);
  mat = glm::translate(mat, glm::vec3(terrain.getX(), 0.0f, terrain.getZ()));
  mat = glm::scale(mat, glm::vec3(1.f, 1.f, 1.f));
  mat = glm::rotate(mat, 0.f, glm::vec3(1.f, 0.f, 0.f));
  mat = glm::rotate(mat, 0.f, glm::vec3(0.f, 1.f, 0.f));
  mat = glm::rotate(mat, 0.f, glm::vec3(0.f, 0.f, 1.f));
  
  _shader.setTransformMat(mat);
}
