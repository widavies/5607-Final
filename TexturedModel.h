#pragma once
#include "RawModel.h"
#include "ModelTexture.h"

// Holds geometry & an associated texture.
class TexturedModel {
public:
  TexturedModel(RawModel model, ModelTexture texture) : _model(model), _texture(texture) {}

  RawModel& getModel() {
    return _model;
  }

  ModelTexture& getTexture() {
    return _texture;
  }
private:
  // Stores the geometry of the textured model
  RawModel _model;
  ModelTexture _texture;
};

