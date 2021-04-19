#pragma once

#include <Constants.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "LTexture.hpp"

class WallGrid {
 public:
  const static int WALL_WIDTH = 32;
  const static int WALL_HEIGHT = 32;
  const static int GRID_COL = SCREEN_WIDTH / WALL_WIDTH;
  const static int GRID_ROW = SCREEN_HEIGHT / WALL_HEIGHT;

  WallGrid();
  void init(SDL_Renderer* renderer);
  void render();

  // Set the wall at position (i,j)
  void set_wall(int i, int j);

  // Remove the wall at position (i,j)
  void unset_wall(int i, int j);

 private:
  LTexture wall_texture;

  // The wall matrix, true indicates wall is present
  bool walls[GRID_ROW][GRID_COL];
};