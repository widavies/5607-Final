#pragma once
#include "RawModel.h"
#include "DisplayManager.h"

// Basic utility rendering functions
class Renderer {

public:
  Renderer(DisplayManager& dm) {
    _dm = &dm;
  }

  void prepare();

  // Renders a RawModel to the buffer
  void render(RawModel& model);

  // Flushes all rendering buffers to the screen
  void flush();
private:
  DisplayManager* _dm;

};