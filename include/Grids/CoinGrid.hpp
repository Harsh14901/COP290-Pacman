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

class CoinGrid : public ObjectGrid {
 public:
  static CoinGrid* getInstance();
  void generate() override;

 protected:
  CoinGrid();

 private:
  static CoinGrid* _instance;
};