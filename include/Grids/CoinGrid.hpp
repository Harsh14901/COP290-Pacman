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
#include "Utils/AssetManager.hpp"

class CoinGrid : public ObjectGrid {
 public:
  CoinGrid();

  static CoinGrid* getInstance();
  void generate() override;

 private:
  static unique_ptr<CoinGrid> _instance;
};