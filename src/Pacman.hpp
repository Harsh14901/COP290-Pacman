#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include "Collision/CollisionEngine.hpp"
#include "LTexture.hpp"
#include "Constants.hpp"
#include "Character.hpp"

#include <stdlib.h> /* srand, rand */
#include <time.h>	/* time */
#include <WallGrid.hpp>

class Pacman : public Character{
public:
  Pacman();
  void handleEvent(SDL_Event& e);
};

