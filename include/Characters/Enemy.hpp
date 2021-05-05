#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */

#include <iostream>

#include "Characters/Character.hpp"
#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "Grids/WallGrid.hpp"
#include "Textures/LTexture.hpp"

enum class EnemyState { NORMAL, WEAK, SUPER_ACTIVE };

class Enemy : public Character {
 public:
  Enemy();
  void render();
  void init(SDL_Renderer* renderer, int enemy_type);
  void handleEvent(SDL_Event& e);
  void move();
  int type;
  EnemyState state = EnemyState::NORMAL;
  void setState(EnemyState st);

 protected:
  void handle_collision();
};