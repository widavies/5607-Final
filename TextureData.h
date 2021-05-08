#pragma once
class TextureData {

public:
  TextureData(unsigned char* data, int width, int height) : 
    _data(data), _width(width), _height(height) {

  }

  int getWidth() {
    return _width;
  }

  int getHeight() {
    return _height;
  }

  unsigned char* getData() {
    return _data;
  }

private:
  unsigned char* _data;
  int _width, _height;

};

