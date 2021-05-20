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
#include "Textures/Particle.hpp"
#include "Utils/Animator.hpp"
#include "Utils/AudioAsset.hpp"
#include "Weapons/Weapon.hpp"
#include "utils.hpp"

class Pacman : public Character {
 public:
  Pacman();

  static Pacman* getInstance();
  static void clearInstance();
  void move();
  void handleEvent(SDL_Event& e);
  int get_coins_collected();
  int get_active_points();
  int get_total_points();
  void render();

  void init(SDL_Renderer* renderer);

  bool is_dead = false;
  bool is_invisible = false;

 protected:
  int coins = 0;
  int cherries = 0;
  int activePoints = 0;
  int totalPoints = 0;

  int BOOST_VEL;
  bool is_boosted = false;
  void init_targets();
  void target_hit(string target_id, Collider* collider = nullptr);

  // void handle_packets();
  // void broadcast_coordinates();
  Packet make_packet(unordered_map<string, string>& data) override;
  void process_packet(Packet& packet) override;

  void boost();
  void check_boost();

 private:
  static unique_ptr<Pacman> _instance;
  bool isMouthOpen();
  void incrementActivePoints(int inc);
  int getInvisibleAlphaValue();

  void collect_coins(int i, int j);
  void collect_cherries(int i, int j);
  void enemy_collision(int num);

  vector<unique_ptr<Particle>> particles;
  void renderParticles();

  void make_invisible();

  AudioAsset chompSound;

  Animator boostAnimator;
  Animator gulp_animator = Animator(3);
  Animator invisibleAnimator = Animator(300);
  SDL_Renderer* _gRenderer;
};
