#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "Grids/ObjectGrid.hpp"
#include "Network/NetworkManager.hpp"
#include "Textures/LTexture.hpp"
class WallGrid : public ObjectGrid {
 public:
  static WallGrid* getInstance();

  // Get an empty location in the matrix
  SDL_Point get_empty_location();

  // Check if a character at (posX, posY) can move in the direction d
  bool can_move(int posX, int posY, Direction d);

  // Generate a random maze
  void generate();

 protected:
  WallGrid();

 private:
  static WallGrid* _instance;
};