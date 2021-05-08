#pragma once
#include <string>
#include "SkyboxShader.h"
#include "RawModel.h"
#include "ModelLoader.h"
#include <vector>

#define SIZE 500.f

class SkyboxRenderer {
public:
  SkyboxRenderer(ModelLoader& loader, glm::mat4 projectionMatrix) {
    std::vector<std::string> files;
    files.push_back("textures/right.png");
    files.push_back("textures/left.png");
    files.push_back("textures/top.png");
    files.push_back("textures/bottom.png");
    files.push_back("textures/back.png");
    files.push_back("textures/front.png");

    float VERTICES[] = {
    -SIZE,  SIZE, -SIZE,
    -SIZE, -SIZE, -SIZE,
     SIZE, -SIZE, -SIZE,
     SIZE, -SIZE, -SIZE,
     SIZE,  SIZE, -SIZE,
    -SIZE,  SIZE, -SIZE,

    -SIZE, -SIZE,  SIZE,
    -SIZE, -SIZE, -SIZE,
    -SIZE,  SIZE, -SIZE,
    -SIZE,  SIZE, -SIZE,
    -SIZE,  SIZE,  SIZE,
    -SIZE, -SIZE,  SIZE,

     SIZE, -SIZE, -SIZE,
     SIZE, -SIZE,  SIZE,
     SIZE,  SIZE,  SIZE,
     SIZE,  SIZE,  SIZE,
     SIZE,  SIZE, -SIZE,
     SIZE, -SIZE, -SIZE,

    -SIZE, -SIZE,  SIZE,
    -SIZE,  SIZE,  SIZE,
     SIZE,  SIZE,  SIZE,
     SIZE,  SIZE,  SIZE,
     SIZE, -SIZE,  SIZE,
    -SIZE, -SIZE,  SIZE,

    -SIZE,  SIZE, -SIZE,
     SIZE,  SIZE, -SIZE,
     SIZE,  SIZE,  SIZE,
     SIZE,  SIZE,  SIZE,
    -SIZE,  SIZE,  SIZE,
    -SIZE,  SIZE, -SIZE,

    -SIZE, -SIZE, -SIZE,
    -SIZE, -SIZE,  SIZE,
     SIZE, -SIZE, -SIZE,
     SIZE, -SIZE, -SIZE,
    -SIZE, -SIZE,  SIZE,
     SIZE, -SIZE,  SIZE
    };

    _cube = loader.loadRaw2D(VERTICES, 108, 3);
    _textureId = loader.loadCubeMap(files);

    _shader = new SkyboxShader;
    _shader->start();
    _shader->setProjectionMat(projectionMatrix);
    _shader->stop();
  }

  ~SkyboxRenderer() {
    delete _shader;
  }

  void render(Camera& camera) {
    _shader->start();
    _shader->setViewMat(camera);
    glBindVertexArray(_cube.getVaoId());
    glEnableVertexAttribArray(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _textureId);
    glDrawArrays(GL_TRIANGLES, 0, _cube.getVertexCount());
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    _shader->stop();
  }

private:
  RawModel _cube;
  GLuint _textureId;
  SkyboxShader* _shader;

};

