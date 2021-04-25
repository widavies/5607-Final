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
using namespace std;

bool saveOutput = false;
float timePast = 0;

// Shader sources
const GLchar* vertexSource =
"#version 150 core\n"
"in vec3 position;"
//"in vec3 inColor;"
"uniform vec3 inColor;"
"in vec3 inNormal;"
"out vec3 Color;"
"out vec3 normal;"
"out vec3 pos;"
"uniform mat4 model;"
"uniform mat4 view;"
"uniform mat4 proj;"
"void main() {"
"   Color = inColor;"
"   gl_Position = proj * view * model * vec4(position,1.0);"
"   pos = (model * vec4(position,1.0)).xyz;"
"   vec4 norm4 = transpose(inverse(model)) * vec4(inNormal,0.0);"  //Or Just model matrix, then noramlize the normal
"   normal = normalize(norm4.xyz);"
"}";

const GLchar* fragmentSource =
"#version 150 core\n"
"in vec3 Color;"
"in vec3 normal;"
"in vec3 pos;"
"out vec4 outColor;"
"const vec3 lightDir = normalize(vec3(1,1,1));"
"const float ambient = .3;"
"void main() {"
"   vec3 diffuseC = Color*max(dot(lightDir,normal),0.0);"
"   vec3 ambC = Color*ambient;"
"   vec3 reflectDir = reflect(lightDir,normal);"
"   vec3 viewDir = normalize(-pos);" //We know the eye is at (0,0)!
"   float spec = max(dot(reflectDir,viewDir),0.0);"
"   if (dot(lightDir,normal) <= 0.0)spec = 0;"
"   vec3 specC = vec3(1.0,1.0,1.0)*pow(spec,4);"
"   outColor = vec4(diffuseC+ambC+specC, 1.0);"
"}";

bool fullscreen = false;
int screen_width = 1200;
int screen_height = 700;

char window_title[] = "My OpenGL Program";

float avg_render_time = 0;

glm::vec3 playerPosition(3.f, 0.f, 0.f);
glm::vec3 playerFacing(0.f, 0.f, 0.f);
glm::vec3 playerUp(0.f, 0.f, 1.f);
glm::vec3 playerVerticalVelocity(0.f, 0.f, 0.f);

bool movingLeft = false, movingRight = false, movingForward = false, movingBack = false;

bool hasA = false, hasB = false, hasC = false, hasD = false, hasE = false;

void Win2PPM(int width, int height);

float powerUp = 1.f;

char** loadMap(string path, int* width, int* height) {
  ifstream file;
  file.open(path);

  if(!file) {
    cout << "Failed to load map" << endl;
    exit(-1);
  }

  file >> *width; file >> *height;

  *width += 2;
  *height += 2;

  char** map = new char* [*height];

  for(int row = 0; row < *height; row++) {
    map[row] = new char[*width];

    for(int col = 0; col < *width; col++) {
      map[row][col] = 'W';
    }
  }

  for(int row = 1; row < *height - 1; row++) {
    for(int col = 1; col < *height - 1; col++) {
      file >> map[row][col];

      if(map[row][col] == 'S') {
        playerPosition.x = row;
        playerPosition.y = col;
      }
    }
  }

  return map;
}

GLuint* loadModel(string path, GLuint shaderProgram, float* triangles) {
  ifstream modelFile;
  modelFile.open(path);
  int numLines = 0;
  modelFile >> numLines;
  float* modelData = new float[numLines];
  for(int i = 0; i < numLines; i++) {
    modelFile >> modelData[i];
  }
  printf("Mode line count: %d\n", numLines);
  float numTris = numLines / 8;

  GLuint* vao = new GLuint;
  glGenVertexArrays(1, vao); //Create a VAO
  glBindVertexArray(*vao); //Bind the above created VAO to the current context

  GLuint vbo[1];
  glGenBuffers(1, vbo);  //Create 1 buffer called vbo
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); //Set the vbo as the active array buffer (Only one buffer can be active at a time)
  glBufferData(GL_ARRAY_BUFFER, numLines * sizeof(float), modelData, GL_STATIC_DRAW); //upload vertices to vbo
  //GL_STATIC_DRAW means we won't change the geometry, GL_DYNAMIC_DRAW = geometry changes infrequently
  //GL_STREAM_DRAW = geom. changes frequently.  This effects which types of GPU memory is used

  //Tell OpenGL how to set fragment shader input 
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
  //Attribute, vals/attrib., type, normalized?, stride, offset
  //Binds to VBO current GL_ARRAY_BUFFER 
  glEnableVertexAttribArray(posAttrib);

  //GLint colAttrib = glGetAttribLocation(shaderProgram, "inColor");
  //glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
  //glEnableVertexAttribArray(colAttrib);

  GLint normAttrib = glGetAttribLocation(shaderProgram, "inNormal");
  glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
  glEnableVertexAttribArray(normAttrib);

  *triangles = numTris;

  return vao;
}

void renderModel(GLint* locModel, GLint* locColor, GLuint* vao, int row, int col, float height, int triangles, float scale, glm::vec3 color) {
  glUniform3fv(*locColor, 1, glm::value_ptr(color));

  glBindVertexArray(*vao);
  glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(row, col, height));
  model = glm::scale(model, glm::vec3(scale, scale, scale));
  glUniformMatrix4fv(*locModel, 1, GL_FALSE, glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, triangles);

}

int main(int argc, char* argv[]) {
  int mapWidth, mapHeight;
  char** map = loadMap("map.txt", &mapWidth, &mapHeight);

  cout << "Map size: " << mapWidth << "x" << mapHeight << endl;

  for(int row = 0; row < mapHeight; row++) {
    for(int col = 0; col < mapWidth; col++) {
      cout << map[row][col];
    }
    cout << endl;
  }


  SDL_Init(SDL_INIT_VIDEO);  //Initialize Graphics (for OpenGL)

  //Ask SDL to get a recent version of OpenGL (3.2 or greater)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  //Create a window (offsetx, offsety, width, height, flags)
  SDL_Window* window = SDL_CreateWindow(window_title, 100, 100, screen_width, screen_height, SDL_WINDOW_OPENGL);
  float aspect = screen_width / (float)screen_height; //aspect ratio (needs to be updated if the window is resized)

  //The above window cannot be resized which makes some code slightly easier.
  //Below show how to make a full screen window or allow resizing
  //SDL_Window* window = SDL_CreateWindow(window_title, 0, 0, screen_width, screen_height, SDL_WINDOW_FULLSCREEN|SDL_WINDOW_OPENGL);
  //SDL_Window* window = SDL_CreateWindow(window_title, 100, 100, screen_width, screen_height, SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
  //SDL_Window* window = SDL_CreateWindow(window_title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,0,0,SDL_WINDOW_FULLSCREEN_DESKTOP|SDL_WINDOW_OPENGL); //Boarderless window "fake" full screen

  //Create a context to draw in
  SDL_GLContext context = SDL_GL_CreateContext(window);
  SDL_SetRelativeMouseMode(SDL_TRUE);

  if(gladLoadGLLoader(SDL_GL_GetProcAddress)) {
    printf("\nOpenGL loaded\n");
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n\n", glGetString(GL_VERSION));
  } else {
    printf("ERROR: Failed to initialize OpenGL context.\n");
    return -1;
  }

  //Load the vertex Shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  //Let's double check the shader compiled 
  GLint status;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
  if(!status) {
    char buffer[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
      "Compilation Error",
      "Failed to Compile: Check Consol Output.",
      NULL);
    printf("Vertex Shader Compile Failed. Info:\n\n%s\n", buffer);
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  //Double check the shader compiled 
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
  if(!status) {
    char buffer[512];
    glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
      "Compilation Error",
      "Failed to Compile: Check Consol Output.",
      NULL);
    printf("Fragment Shader Compile Failed. Info:\n\n%s\n", buffer);
  }

  //Join the vertex and fragment shaders together into one program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor"); // set output
  glLinkProgram(shaderProgram); //run the linker

  glUseProgram(shaderProgram); //Set the active shader (only one can be used at a time)


  //Build a Vertex Array Object. This stores the VBO and attribute mappings in one object

  //Allocate memory on the graphics card to store geometry (vertex buffer object)
  float trisCube;
  GLuint* vaoCube = loadModel("models/cube.txt", shaderProgram, &trisCube);
  float trisKnot;
  GLuint* vaoKnot = loadModel("models/knot.txt", shaderProgram, &trisKnot);
  float trisTeapot;
  GLuint* vaoTeapot = loadModel("models/teapot.txt", shaderProgram, &trisTeapot);

  glBindVertexArray(0); //Unbind the VAO

  //You need a second VAO, e.g., if some of the models are stored in a second format
  //Here is what that looks like--
  //GLuint vao2;  
  //glGenVertexArrays(1, &vao2); //Create the VAO
  //glBindVertexArray(vao2); //Bind the above created VAO to the current context
  //  Creat VBOs ... 
  //  Set-up attributes ...
  //glBindVertexArray(0); //Unbind the VAO


  glEnable(GL_DEPTH_TEST);

  //Event Loop (Loop forever processing each event as fast as possible)
  SDL_Event windowEvent;
  bool quit = false;

  Uint32 lastTick = 0, delta = 0;

  while(!quit) {
    float t_start = SDL_GetTicks();

    int mouseXDelta = 0;
    int mouseYDelta = 0;


    while(SDL_PollEvent(&windowEvent)) {
      if(windowEvent.type == SDL_QUIT) quit = true; //Exit event loop
      //List of keycodes: https://wiki.libsdl.org/SDL_Keycode - You can catch many special keys
      //Scancode referes to a keyboard position, keycode referes to the letter (e.g., EU keyboards)
      if(windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE)
        quit = true; //Exit event loop
      if(windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_f) //If "f" is pressed
        fullscreen = !fullscreen;
      SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0); //Set to full screen 


      if(windowEvent.type == SDL_KEYDOWN) {
        switch(windowEvent.key.keysym.sym) {
        case SDLK_w:
          movingForward = true;
          break;
        case SDLK_s:
          movingBack = true;
          break;
        case SDLK_a:
          movingLeft = true;
          break;
        case SDLK_d:
          movingRight = true;
          break;
        case SDLK_SPACE:
          if(playerPosition.z < 0.5) {
            playerVerticalVelocity.z = 0.03;
          }
          break;
        }
      } else if(windowEvent.type == SDL_KEYUP) {
        switch(windowEvent.key.keysym.sym) {
        case SDLK_w:
          movingForward = false;
          break;
        case SDLK_s:
          movingBack = false;
          break;
        case SDLK_a:
          movingLeft = false;
          break;
        case SDLK_d:
          movingRight = false;
          break;

        }
      }

      if(windowEvent.type == SDL_MOUSEMOTION) {
        mouseXDelta = windowEvent.motion.xrel;
        mouseYDelta = windowEvent.motion.yrel;
      }
    }

    // Clear the screen to default color
    glClearColor(.2f, 0.4f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(mouseXDelta != 0) {
      glm::mat4 rotateYaw = glm::rotate(glm::mat4(1.0f), mouseXDelta * 0.009f, playerUp);
      playerFacing = glm::vec3(glm::vec4(playerFacing - playerPosition, 1.0f) * rotateYaw);
    }

    if(mouseYDelta != 0) {
      glm::vec3 right = glm::cross(playerFacing - playerPosition, playerUp);
      glm::mat4 rotatePitch = glm::rotate(glm::mat4(1.0f), mouseYDelta * 0.009f, right);
      playerFacing = glm::vec3(glm::vec4(playerFacing - playerPosition, 1.0f) * rotatePitch);
      //playerUp = glm::cross(right, playerFacing - playerPosition);
    }

    // Compute vector for movement
    float at = 0;
    float strafe = 0;
    if(movingForward) at += (delta / 1000.f);
    if(movingBack) at -= (delta / 1000.f);
    if(movingLeft) strafe -= (delta / 1000.f);
    if(movingRight) strafe += (delta / 1000.f);

    glm::vec3 unit = glm::normalize(playerFacing - playerPosition);
    unit.z = 0;
    playerPosition += unit * at * powerUp;
    /*playerPosition = glm::vec3(playerFacing);
    playerPosition.x -= 3.f;*/
    glm::vec3 right = glm::normalize(glm::cross(playerFacing - playerPosition, playerUp));
    right.z = 0;
    playerPosition += right * strafe * powerUp;

    playerVerticalVelocity.z -= 0.1 * (delta / 1000.f);

    playerPosition.z += playerVerticalVelocity.z;

    if(playerPosition.z < 0) playerPosition.z = 0;

    for(int row = 0; row < mapHeight; row++) {
      for(int col = 0; col < mapWidth; col++) {
        if(map[row][col] != '0' && map[row][col] != 'S') {

          // Check if the player is colliding 
          if(playerPosition.x > row - 0.55 && playerPosition.x < row + 0.55 && playerPosition.y > col - 0.55 && playerPosition.y < col + 0.55) {
            if(map[row][col] == 'a') {
              hasA = true;
              map[row][col] = '0';
            }             else if(map[row][col] == 'b') {
              hasB = true;
              map[row][col] = '0';
            }             else if(map[row][col] == 'c') {
              hasC = true;
              map[row][col] = '0';
            }             else if(map[row][col] == 'd') {
              hasD = true;
              map[row][col] = '0';
            }             else if(map[row][col] == 'e') {
              hasE = true;
              map[row][col] = '0';
            }             else if(map[row][col] == 'A' && hasA) {
              hasA = false;
              map[row][col] = '0';
            }             else if(map[row][col] == 'B' && hasB) {
              hasB = false;
              map[row][col] = '0';
            }             else if(map[row][col] == 'C' && hasC) {
              hasC = false;
              map[row][col] = '0';
            }             else if(map[row][col] == 'D' && hasD) {
              hasD = false;
              map[row][col] = '0';
            }             else if(map[row][col] == 'E' && hasE) {
              hasE = false;
              map[row][col] = '0';
            }             else if(map[row][col] == 'P') {
              powerUp += 1.f;
              map[row][col] = '0';
            }             else if(map[row][col] == 'G') {
              // Move player back to the start
              for(int row2 = 0; row2 < mapHeight; row2++) {
                for(int col2 = 0; col2 < mapWidth; col2++) {
                  if(map[row2][col2] == 'S') {
                    playerPosition.x = row2;
                    playerPosition.y = col2;
                  }
                }
              }
            } else {
              if(std::abs(playerPosition.x - row) < std::abs(playerPosition.y - col)) {
                if(playerPosition.y > col - 0.55 && playerPosition.y < col) {
                  playerPosition.y = col - 0.56;
                } else if(playerPosition.y < col + 0.55 && playerPosition.y > col) {
                  playerPosition.y = col + 0.56;
                }
              } else {
                if(playerPosition.x < row + 0.55 && playerPosition.x > row) {
                  playerPosition.x = row + 0.56;
                } else if(playerPosition.x > row - 0.55 && playerPosition.x < row) {
                  playerPosition.x = row - 0.56;
                }
              }
            }
          }
        }
      }
    }

    if(!saveOutput) timePast = SDL_GetTicks() / 1000.f;
    if(saveOutput) timePast += .07; //Fix framerate at 14 FPS
    glm::mat4 model = glm::mat4(1);
    /*model = glm::rotate(model, timePast * 3.14f / 2, glm::vec3(0.0f, 1.0f, 1.0f));
    model = glm::rotate(model, timePast * 3.14f / 4, glm::vec3(1.0f, 0.0f, 0.0f));*/
    GLint uniModel = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
    glm::mat4 view = glm::lookAt(
      playerPosition,  //Cam Position
      playerFacing,  //Look at point
      playerUp); //Up
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 proj = glm::perspective(3.14f / 4, aspect, 0.01f, 10.0f); //FOV, aspect, near, far
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

    glBindVertexArray(*vaoCube);
    //glDrawArrays(GL_TRIANGLES, 0, numTris); //(Primitives, Which VBO, Number of vertices)

    GLint colorView = glGetUniformLocation(shaderProgram, "inColor");
    glUniform3fv(colorView, 1, glm::value_ptr(glm::vec3(0.7f, 0.7f, 0.0f)));

    // Render the key in hand
    glm::vec3 key = playerPosition + glm::normalize(playerFacing) * 0.5f;

    if(hasA) {
      renderModel(&uniModel, &colorView, vaoKnot, key.x, key.y, 0.2, trisKnot, .25f, glm::vec3(0.f, 0.7f, 0.0f));
    } else if(hasB) {
      renderModel(&uniModel, &colorView, vaoKnot, key.x, key.y, 0.2, trisKnot, .25f, glm::vec3(0.7f, 0.0f, 0.0f));
    } else if(hasC) {
      renderModel(&uniModel, &colorView, vaoKnot, key.x, key.y, 0.2, trisKnot, .25f, glm::vec3(0.0f, 0.7f, 0.7f));
    } else if(hasD) {
      renderModel(&uniModel, &colorView, vaoKnot, key.x, key.y, 0.2, trisKnot, .25f, glm::vec3(0.35f, 0.7f, 0.25f));
    } else if(hasE) {
      renderModel(&uniModel, &colorView, vaoKnot, key.x, key.y, 0.2, trisKnot, 1.25f, glm::vec3(0.7f, 0.35f, 0.65f));
    }

    // Draw walls, each wall can be drawn as a block.
    // Each block is 1x1.
    for(int row = 0; row < mapHeight; row++) {
      for(int col = 0; col < mapWidth; col++) {
        if(map[row][col] == 'W') {
          renderModel(&uniModel, &colorView, vaoCube, row, col, 0, trisCube, 1.f, glm::vec3(0.7f, 0.7f, 0.0f));
        }

        if(map[row][col] == 'A') {
          renderModel(&uniModel, &colorView, vaoCube, row, col, 0, trisCube, 1.f, glm::vec3(0.0f, 0.7f, 0.0f));
        } else if(map[row][col] == 'B') {
          renderModel(&uniModel, &colorView, vaoCube, row, col, 0, trisCube, 1.f, glm::vec3(0.7f, 0.0f, 0.0f));
        } else if(map[row][col] == 'C') {
          renderModel(&uniModel, &colorView, vaoCube, row, col, 0, trisCube, 1.f, glm::vec3(0.0f, 0.7f, 0.7f));
        } else if(map[row][col] == 'D') {
          renderModel(&uniModel, &colorView, vaoCube, row, col, 0, trisCube, 1.f, glm::vec3(0.35f, 0.7f, 0.25f));
        } else if(map[row][col] == 'E') {
          renderModel(&uniModel, &colorView, vaoCube, row, col, 0, trisCube, 1.f, glm::vec3(0.7f, 0.35f, 0.65f));
        } else if(map[row][col] == 'a' && !hasA) {
          renderModel(&uniModel, &colorView, vaoKnot, row, col, 0, trisKnot, 0.25f, glm::vec3(0.0f, 0.7f, 0.0f));
        } else if(map[row][col] == 'b' && !hasB) {
          renderModel(&uniModel, &colorView, vaoKnot, row, col, 0, trisKnot, 0.25f, glm::vec3(0.7f, 0.0f, 0.0f));
        } else if(map[row][col] == 'c' && !hasC) {
          renderModel(&uniModel, &colorView, vaoKnot, row, col, 0, trisKnot, 0.25f, glm::vec3(0.0f, 0.7f, 0.7f));
        } else if(map[row][col] == 'd' && !hasD) {
          renderModel(&uniModel, &colorView, vaoKnot, row, col, 0, trisKnot, 0.25f, glm::vec3(0.35f, 0.7f, 0.25f));
        } else if(map[row][col] == 'e' && !hasE) {
          renderModel(&uniModel, &colorView, vaoKnot, row, col, 0, trisKnot, 0.25f, glm::vec3(0.7f, 0.35f, 0.65f));
        } else if(map[row][col] == 'P') {
          renderModel(&uniModel, &colorView, vaoTeapot, row, col, 0, trisTeapot, 0.25f, glm::vec3(0.7f, 0.6f, 0.2f));
        }

        // Floor
        if(map[row][col] == 'S') {
          renderModel(&uniModel, &colorView, vaoCube, row, col, -1, trisCube, 1.f, glm::vec3(0.f, 0.0f, 0.7f));
        } else if(map[row][col] == 'G') {
          renderModel(&uniModel, &colorView, vaoCube, row, col, -1, trisCube, 1.f, glm::vec3(0.f, 0.7f, 0.f));
        } else {
          renderModel(&uniModel, &colorView, vaoCube, row, col, -1, trisCube, 1.f, glm::vec3(0.7f, 0.7f, 0.7f));
        }

      }
    }

    if(saveOutput) Win2PPM(screen_width, screen_height);

    SDL_GL_SwapWindow(window); //Double buffering

    Uint32 ticks = SDL_GetTicks();
    delta = ticks - lastTick;
    lastTick = ticks;

    float t_end = SDL_GetTicks();
    char update_title[100];
    float time_per_frame = t_end - t_start;
    avg_render_time = .98 * avg_render_time + .02 * time_per_frame; //Weighted average for smoothing
    sprintf(update_title, "%s [Update: %3.0f ms]\n", window_title, avg_render_time);
    SDL_SetWindowTitle(window, update_title);
  }


  //Clean Up
  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);

  /*glDeleteBuffers(1, vbo);
  glDeleteVertexArrays(1, &vao);*/

  SDL_GL_DeleteContext(context);
  SDL_Quit();
  return 0;
}


//Write out PPM image from screen
void Win2PPM(int width, int height) {
  char outdir[10] = "out/"; //Must be defined!
  int i, j;
  FILE* fptr;
  static int counter = 0;
  char fname[32];
  unsigned char* image;

  /* Allocate our buffer for the image */
  image = (unsigned char*)malloc(3 * width * height * sizeof(char));
  if(image == NULL) {
    fprintf(stderr, "ERROR: Failed to allocate memory for image\n");
  }

  /* Open the file */
  sprintf(fname, "%simage_%04d.ppm", outdir, counter);
  if((fptr = fopen(fname, "w")) == NULL) {
    fprintf(stderr, "ERROR: Failed to open file to write image\n");
  }

  /* Copy the image into our buffer */
  glReadBuffer(GL_BACK);
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

  /* Write the PPM file */
  fprintf(fptr, "P6\n%d %d\n255\n", width, height);
  for(j = height - 1; j >= 0; j--) {
    for(i = 0; i < width; i++) {
      fputc(image[3 * j * width + 3 * i + 0], fptr);
      fputc(image[3 * j * width + 3 * i + 1], fptr);
      fputc(image[3 * j * width + 3 * i + 2], fptr);
    }
  }

  free(image);
  fclose(fptr);
  counter++;
}