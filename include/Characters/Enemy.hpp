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
#include "Utils/Animator.hpp"
#include "Characters/GhostManager.hpp"

enum class EnemyState { NORMAL, WEAK, SUPER_ACTIVE };

class Enemy : public Character {
 public:
  static void switch_active_id();

  static void make_enemies(int num = ENEMY_COUNT);
  static vector<Enemy*> get_enemies();

  void render();
  void init(SDL_Renderer* renderer, int enemy_type);
  void handleEvent(SDL_Event& e);
  void move();
  int type;
  int id;
  EnemyState state = EnemyState::NORMAL;
  void setState(EnemyState st);
  void respawn();

 protected:
  Enemy(int type);
  int getEnemyColor();


  void handle_collision();
  // The list of enemy ids
  static vector<int> ids;

  // List of all the enemies
  static vector<Enemy*> enemies;
  // The id that is being controlled by player 2
  static int active_id;

 private:
  void randomize_direction();
  Animator weak_state_animator = Animator(400);
};