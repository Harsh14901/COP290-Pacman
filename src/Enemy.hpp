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

enum class EnemyState { NORMAL,WEAK,SUPER_ACTIVE };


class Enemy: public Character{
public:
  Enemy();
  void render();
  void init(SDL_Renderer* renderer,int enemy_type);
  void handleEvent(SDL_Event& e);
  void move();
  int type;
  EnemyState state = EnemyState::NORMAL;
  void setState(EnemyState st);
protected:
  void handle_collision();
  
};