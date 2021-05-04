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
#include "CherryGrid.hpp"
#include "utils.hpp"
#include "Animator.hpp"
#include "Enemy.hpp"

class Pacman : public Character{
public:
  Pacman();
  void move();
  void handleEvent(SDL_Event& e);
  int get_coins_collected();
  void render();
  bool is_dead = false;

protected:
  void handle_collision();
private:
  Animator gulp_animator = Animator(3);
  bool isMouthOpen();

  int coins;
  int cherries;

};

