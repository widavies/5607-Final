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
#include "StaticShader.h"
#include "ModelTexture.h"
#include "TexturedModel.h"
#include "OBJLoader.h"
#include "MasterRenderer.h"
#include "RawModel.h"

using namespace std;

bool turnup = false;
bool turndown = false;
bool turnleft = false;
bool turnright = false;
bool turnclockwise = false;
bool turnccw = false;

float playerSpeed = .01;
float turnSpeed = 0.02;

float cameraPitch = 0;


void keyA(bool up) {
  //cout << "Space pressed" << endl;
  if (!up) {
      turnleft = true;
  }
  else {
      turnleft = false;
  }

}

void keyW(bool up) {
    //cout << "Space pressed" << endl;
    if (!up) {
        turnup = true;
    }
    else {
        turnup = false;
    }
}

void keyS(bool up) {
    //cout << "Space pressed" << endl;
    if (!up) {
        turndown = true;
    }
    else {
        turndown = false;
    }
}

void keyD(bool up) {
    //cout << "Space pressed" << endl;
    if (!up) {
        turnright = true;
    }
    else {
        turnright = false;
    }
}

void keyQ(bool up) {
    //cout << "Space pressed" << endl;
    if (!up) {
        turnccw = true;
    }
    else {
        turnccw = false;
    }
}

void keyE(bool up) {
    //cout << "Space pressed" << endl;
    if (!up) {
        turnclockwise = true;
    }
    else {
        turnclockwise = false;
    }
}


int main(int argc, char* argv[]) {
    cout << "gets to here";
  DisplayManager dm("Project 4", 1600, 900, false, false);
  ModelLoader modelLoader;

  ModelTexture grass = modelLoader.loadTexture("textures/grass.jpg");

  Camera camera;

  // Add key listeners here
  dm.AddKeyListener(SDLK_a, keyA);
  dm.AddKeyListener(SDLK_s, keyS);
  dm.AddKeyListener(SDLK_d, keyD);
  dm.AddKeyListener(SDLK_w, keyW);
  dm.AddKeyListener(SDLK_q, keyQ);
  dm.AddKeyListener(SDLK_e, keyE);

  RawModel square = OBJLoader::loadOBJ("models/VLJ19.obj", modelLoader);
  ModelTexture texture = modelLoader.loadTexture("models/CIRRUSTS19.jpg", 10.f, 1.f);

  Terrain terrain1(0, -1, modelLoader, &grass);
  Terrain terrain2(-1, -1, modelLoader, &grass);

  TexturedModel model(square, texture);
   

  Entity entity(&model, 0.f, 0.f, -15.f);
  entity.rotate(0, 3.1415f, 0.0f);

  Light light(glm::vec3(0.f, 5.f, -15.f), glm::vec3(1.f, 1.f, 1.f));
  float pi = 3.1415;

  MasterRenderer* renderer = new MasterRenderer(dm);

  while(dm.Update()) {
    //entity.translate(0.f, 0.f, -0.002f);
    // 
    //entity.rotate(0.f, 0.02f, 0.f);
    //camera.move(0.0f, 0.f, 0.002f);

    if (turnup) {
        entity.rotate(0.02f, 0.0f, 0.0f);
    }
    if (turndown) {
        entity.rotate(-0.02f, 0.0f, 0.0f);
    }
    if (turnleft) {
        entity.rotate(0.0f, 0.02f, 0.0f);
    }
    if (turnright) {
        entity.rotate(0.0f, -0.02f, 0.0f);
    }
    if (turnclockwise) {
        entity.rotate(0.0f, 0.0f, 0.02f);
    }
    if (turnccw) {
        entity.rotate(0.0f, 0.0f, -0.02f);
    }


    entity.translate(playerSpeed * sin(entity.getRotateY()), playerSpeed * sin(entity.getRotateX()), playerSpeed * cos(entity.getRotateY() ));


    camera.calculatePitch(dm.mouseYDelta);
    camera.calculateAngleAroundPlayer(dm.mouseXDelta);
    
    float horizontalDistance = camera.calculateHorizontalDistance();
    float verticalDistance = camera.calculateVerticalDistance();
    glm::vec3 tmp = entity.getPos();
    camera.calculateCameraPosition(horizontalDistance, verticalDistance, tmp.x, tmp.y, tmp.z, entity.getRotateY());
    dm.mouseYDelta = 0;
    dm.mouseXDelta = 0;
    //camera.calculateCameraPosition(verticalDistance, entity.getPos().y);

    //cout << camera.getPosition().x << ", " << camera.getPosition().y << ", " << camera.getPosition().z << "  \n";
    //cout << "pitch: " << camera.getPitch() << "\n";

    renderer->prepare();
    //entity.rotate(0.f, 0.02f, 0.f);
    //camera.move(0.0f, 0.02f, 0.f);

    renderer->queueEntity(entity);
    renderer->queueTerrain(terrain1);
    renderer->queueTerrain(terrain2);

    renderer->render(light, camera);

  }

  modelLoader.close();

  delete renderer;

  return 0;
}