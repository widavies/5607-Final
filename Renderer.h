#pragma once
#include "RawModel.h"

// Basic utility rendering functions
class Renderer {

public:
  void prepare();

  void render(RawModel& model);

};