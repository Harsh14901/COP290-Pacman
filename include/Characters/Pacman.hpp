#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */

#include <iostream>

#include "Characters/Character.hpp"
#include "Characters/Enemy.hpp"
#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "Grids/CherryGrid.hpp"
#include "Grids/CoinGrid.hpp"
#include "Grids/WallGrid.hpp"
#include "Textures/LTexture.hpp"
#include "Utils/Animator.hpp"
#include "Utils/AudioAsset.hpp"
#include "utils.hpp"

class Pacman : public Character {
 public:
  Pacman();
  void move();
  void handleEvent(SDL_Event& e);
  int get_coins_collected();
  int get_active_points();
  void render();

  void init(SDL_Renderer* renderer);

  bool is_dead = false;
  bool is_invisible = false;

 protected:
  void handle_collision();
  void handle_packets();
  void broadcast_coordinates();

 private:
  Animator gulp_animator = Animator(3);
  bool isMouthOpen();
  void incrementActivePoints(int inc);

  int coins = 0;
  int cherries = 0;
  int activePoints = 0;

  AudioAsset chompSound;

  Animator freezeAnimation = Animator(100);
  Animator invisibleAnimator = Animator(300);
  int getInvisibleAlphaValue();
};
