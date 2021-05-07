#pragma once
#include <string>
#include <iostream>

#include "glad/glad.h" 
#include <SDL.h>
#include <SDL_opengl.h>
#include <map>

// up - true if key lifted, false if key pressed
typedef void KeyListener(bool up);

// Handles SDL2 management (display, keys, etc.)
class DisplayManager {
public:
  /*
   * Creates a display using SDL2
   * width,height - pixel size of window
   * fullscreen - whether to enable fullscreen borderless. This ignores width & height
   * resizable - whether to allow the window to be resized
   */
  DisplayManager(const std::string& title, int width, int height, bool fullscreen, bool resizable);

  void AddKeyListener(SDL_Keycode key, const KeyListener& listener);

  // Returns false if the application should exit
  bool Update();

  void Swap();

  ~DisplayManager();
  float aspect;
private:
  SDL_Window* _window;
  SDL_GLContext _context;
  int _width, _height;

  std::map<SDL_Keycode, const KeyListener&> _keyListeners;
};

