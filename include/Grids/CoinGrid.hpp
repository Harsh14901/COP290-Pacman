#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "Textures/LTexture.hpp"
#include "Network/NetworkManager.hpp"
#include "Grids/ObjectGrid.hpp"

class CoinGrid : public ObjectGrid {
public:
  void generate();
};