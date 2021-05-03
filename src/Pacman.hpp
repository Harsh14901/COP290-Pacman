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
#include "CoinGrid.hpp"
#include "utils.hpp"

class Pacman : public Character{
public:
  Pacman();
  void move();
  void handleEvent(SDL_Event& e);
  int get_coins_collected();
  void render();
protected:
  void handle_collision();
private:
  int coins;
};

