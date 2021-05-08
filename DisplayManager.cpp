#include "DisplayManager.h"


DisplayManager::DisplayManager(const std::string& title, int width, int height, bool fullscreen, bool resizable) 
  : _width(width), _height(height) {

  aspect = width / (float)height;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  Uint32 flags = SDL_WINDOW_OPENGL | (SDL_WINDOW_FULLSCREEN_DESKTOP * fullscreen) | (SDL_WINDOW_RESIZABLE * resizable);
  _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
  _context = SDL_GL_CreateContext(_window);
  
  // Mouse events will be delivered as deltas instead of as absolute
  // positions
  SDL_SetRelativeMouseMode(SDL_TRUE);

  if(gladLoadGLLoader(SDL_GL_GetProcAddress)) {
    printf("\nOpenGL loaded\n");
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n\n", glGetString(GL_VERSION));
  } else {
    printf("ERROR: Failed to initialize OpenGL context.\n");
    exit(-1);
  }

}

void DisplayManager::AddKeyListener(SDL_Keycode key, const KeyListener& listener) {
  _keyListeners.insert(std::pair<SDL_Keycode, const KeyListener&>(key, listener));
}

bool DisplayManager::Update() {
  SDL_Event windowEvent;

  while(SDL_PollEvent(&windowEvent)) {
    if(windowEvent.type == SDL_QUIT) {
      return false;
    } else if(windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) {
      return false;
    } else if(windowEvent.type == SDL_KEYDOWN || windowEvent.type == SDL_KEYUP) {
      std::map<SDL_Keycode, const KeyListener&>::iterator it = _keyListeners.find(windowEvent.key.keysym.sym);

      if(it != _keyListeners.end()) {
        it->second(windowEvent.type == SDL_KEYUP);
      }
    }

    if(windowEvent.type == SDL_MOUSEMOTION) {
      mouseXDelta = windowEvent.motion.xrel;
      mouseYDelta = windowEvent.motion.yrel;
    }
  }

  return true;
}

void DisplayManager::Swap() {

  SDL_GL_SwapWindow(_window);

}

DisplayManager::~DisplayManager() {

  SDL_GL_DeleteContext(_context);
  SDL_Quit();

}

// Notes:
// vao
// stores data about a 3d object
// attribute lists - different sets of data
  // vertex positions, colors, normals, texture coordinates, etc. (totally up to you)
  // vbo: one of the above. positions, colors, positions. Each one is loaded into an attribute list.
  // each vao has an id
