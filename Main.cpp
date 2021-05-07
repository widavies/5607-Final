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
#include "OBJLoader.h"
#include "MasterRenderer.h"

using namespace std;

void keyA(bool up) {
  cout << "Space pressed" << endl;

}

int main(int argc, char* argv[]) {
  DisplayManager dm("Project 4", 1600, 900, false, false);
  ModelLoader modelLoader;

  Camera camera;

  // Add key listeners here
  dm.AddKeyListener(SDLK_a, keyA);

  RawModel square = OBJLoader::loadOBJ("models/stall.obj", modelLoader);
  ModelTexture texture = modelLoader.loadTexture("models/stallTexture.jpg", 10.f, 1.f);

  TexturedModel model(square, texture);
   
  Entity entity(&model, 0.f, 0.f, -15.f);
  Light light(glm::vec3(0.f, 0.f, -20.f), glm::vec3(1.f, 1.f, 1.f));

  MasterRenderer* renderer = new MasterRenderer(dm);

  while(dm.Update()) {
    //entity.translate(0.f, 0.f, -0.002f);
    // 
    entity.rotate(0.f, 0.02f, 0.f);
    //camera.move(0.0f, 0.f, 0.002f);

    renderer->queueEntity(entity);

    renderer->render(light, camera);

  }

  modelLoader.close();

  delete renderer;

  return 0;
}