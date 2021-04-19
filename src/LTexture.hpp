#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "Constants.h"

using namespace std;

class LTexture {
 public:
  // Initializes variables
  LTexture(SDL_Renderer* renderer);
  LTexture();
  void setRenderer(SDL_Renderer* renderer);

  // Deallocates memory
  ~LTexture();

  // Loads image at specified path
  bool loadFromFile(std::string path);

#if defined(SDL_TTF_MAJOR_VERSION)
  // Creates image from font string
  bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

  // Deallocates texture
  void free();

  // Set color modulation
  void setColor(Uint8 red, Uint8 green, Uint8 blue);

  // Set blending
  void setBlendMode(SDL_BlendMode blending);

  // Set alpha modulation
  void setAlpha(Uint8 alpha);

  // Renders texture at given point
  void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0,
              SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

  // Override the image dimensions
  void set_image_dimenstions(int width, int height);

  // Gets image dimensions
  int getWidth();
  int getHeight();

 private:
  // The actual hardware texture
  SDL_Texture* mTexture;
  SDL_Renderer* _gRenderer;

  // Image dimensions
  int mWidth;
  int mHeight;
};