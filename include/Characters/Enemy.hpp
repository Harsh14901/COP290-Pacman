#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */

#include <iostream>

#include "AI/GhostAIEngine.hpp"
#include "Characters/Character.hpp"
#include "Characters/GhostManager.hpp"
#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "Grids/WallGrid.hpp"
#include "Textures/LTexture.hpp"
#include "Utils/Animator.hpp"
#include "Weapons/FreezeBullet.hpp"
#include "Weapons/Weapon.hpp"

class Enemy : public Character {
 public:
  Enemy(int type);
  static void switch_active_id();
  static int get_active_id();
  static Enemy* get_active_enemy();

  static void make_enemies(int num = ENEMY_COUNT);
  static vector<Enemy*> get_enemies();

  void render();
  void init(SDL_Renderer* renderer);
  void handleEvent(SDL_Event& e);
  void move();
  void setState(EnemyState st);
  void respawn();

  EnemyState state = EnemyState::NORMAL;

 protected:
  int id;
  int type;
  static int active_index;
  static vector<unique_ptr<Enemy>> enemies;

  int getEnemyColor();
  void handle_collision();

 private:
  void randomize_direction();

  GhostAIEngine AIEngine;
  Animator weak_state_animator = Animator(400);
  Animator spawnAnimator = Animator(100);
  Weapon weapon;
};