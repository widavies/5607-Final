//CSCI 5607 OpenGL Tutorial (HW 4)
//Loading 3D Models

#include "glad/glad.h"  //Include order can matter here
#include <SDL.h>
#include <SDL_opengl.h>
#include <cstdio>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include "DisplayManager.h"
#include "ModelLoader.h"
#include "Renderer.h"
#include "StaticShader.h"
#include "ModelTexture.h"
#include "TexturedModel.h"
using namespace std;

void keySpace(bool up) {
  cout << "Space pressed" << endl;
}

int main(int argc, char* argv[]) {
  DisplayManager dm("Project 4", 1600, 900, false, false);
  ModelLoader modelLoader;
  Renderer renderer(dm);

  StaticShader * shaders = new StaticShader();

  // Add key listeners here
  dm.AddKeyListener(SDLK_SPACE, keySpace);

  float vertices[] = {
      -0.5f, 0.5f, 0.f,//v0
      -0.5f, -0.5f, 0.f,//v1
      0.5f, -0.5f, 0.f,//v2
      0.5f, 0.5f, 0.f,//v3
  };

  int indices[] = {
      0,1,3,//top left triangle (v0, v1, v3)
      3,1,2//bottom right triangle (v3, v1, v2)
  };

  float textureCoords[] = {
    0, 0, // v0
    0, 1, // v1
    1, 1, // v2
    1, 0 // v3
  };

  RawModel square = modelLoader.loadRaw(vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(int), textureCoords, sizeof(textureCoords) / sizeof(float));
  ModelTexture texture = modelLoader.loadTexture("textures/container.jpg");
  TexturedModel model(square, texture);

  while(dm.Update()) {
    renderer.prepare();

    shaders->start();

    renderer.render(model);

    shaders->stop();

    renderer.flush();
  }

  modelLoader.close();
  delete shaders;

  return 0;
}