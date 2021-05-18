#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <Constants.hpp>
#include <iostream>

#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "Grids/ObjectGrid.hpp"
#include "Network/NetworkManager.hpp"
#include "Textures/LTexture.hpp"
#include "Utils/AssetManager.hpp"

class CherryGrid : public ObjectGrid {
 public:
  static CherryGrid* getInstance();
  void generate() override;

 protected:
  CherryGrid();

 private:
  static CherryGrid* _instance;
};
