#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <Constants.hpp>
#include <iostream>

#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "LTexture.hpp"
#include "Network/NetworkManager.hpp"
#include "ObjectGrid.hpp"

class CherryGrid : public ObjectGrid {
public:
  void generate();
};
