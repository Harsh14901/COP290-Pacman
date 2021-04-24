#pragma once

#include <Constants.hpp>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "LTexture.hpp"
#include "CollisionEngine.hpp"
#include "Constants.hpp"
class WallGrid {
 public:

  // const static int GRID_COL = 28;
  // const static int GRID_ROW = 36;
  const static int WALL_WIDTH = WALL_GRID_WIDTH;
  const static int WALL_HEIGHT = WALL_GRID_HEIGHT;
  // const static int GRID_COL = SCREEN_WIDTH / WALL_WIDTH;
  // const static int GRID_ROW = SCREEN_HEIGHT / WALL_HEIGHT;
  const static int GRID_COL = WALL_GRID_COLS;
  const static int GRID_ROW = WALL_GRID_ROWS;
  const static string WALL_ID;

  WallGrid();
  void init(SDL_Renderer* renderer);
  void render();

  // Set the wall at position (i,j)
  void set_wall(int i, int j);

  // Remove the wall at position (i,j)
  void unset_wall(int i, int j);

  // Get an empty location in the matrix
  SDL_Point get_empty_location();

 private:
  LTexture wall_texture;

  // The wall matrix, true indicates wall is present
  bool walls[GRID_ROW][GRID_COL];
  Collider wallColliders[GRID_ROW][GRID_COL];
};