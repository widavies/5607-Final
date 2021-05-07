#pragma once
#include "RawModel.h"
#include "ModelTexture.h"
#include "ModelLoader.h"

#define SIZE 800
#define VERTEX_COUNT 128

class Terrain {
public:
  Terrain(int gridX, int gridZ, ModelLoader& loader, ModelTexture* texture) : _texture(texture) {
    _x = gridX * SIZE;
    _z = gridZ * SIZE;
    _model = generateTerrain(loader);
  }

  RawModel& getModel() {
    return _model;
  }

  ModelTexture* getTexture() {
    return _texture;
  }

  float getX() {
    return _x;
  }

  float getZ() {
    return _z;
  }


private:
  float _x, _z;
  RawModel _model;
  ModelTexture* _texture;

  RawModel generateTerrain(ModelLoader& loader);

};

