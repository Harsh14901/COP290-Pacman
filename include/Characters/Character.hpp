#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */

#include <iostream>

#include "Base.hpp"
#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "Grids/WallGrid.hpp"
#include "Network/NetworkManager.hpp"
#include "Textures/LTexture.hpp"
#include "Utils/Animator.hpp"
#include "Utils/FrameGuider.hpp"
#include "Weapons/Weapon.hpp"

class Character : public Base {
 public:
  // Initializes the variables
  Character(string id);
  Character(string id, string asset);

  void init(SDL_Renderer* renderer) override;
  void init(SDL_Renderer* renderer,
            pair<BulletType, BulletType> equipped_bullets);

  void init_targets() override;

  // Takes key presses and adjusts the dot's velocity
  void handleEvent(SDL_Event& e) override;

  // Moves the dot
  void move() override;

  // Shows the dot on the screen
  void render() override;

  void change_direction(Direction d) override;

  string get_weapon_text();

 protected:
  // The X and Y offsets of the dot
  void init_collider() override;
  void target_hit(string target_id, Collider* collider = nullptr) override;
  void handle_packets() override;
  void broadcast_coordinates() override;

  void emp(bool broadcast = true);
  void freeze(bool broadcast = true);
  void check_emp();
  void check_frozen();

  virtual Packet make_packet(unordered_map<string, string>& data);
  virtual void process_packet(Packet& packet);

  // The collider associated with pacman
  Direction _next = Direction::NONE;
  string COLLIDER_ID;

  WeaponSet weaponSet;

  Animator empAnimation;
  Animator freezeAnimation;

  pair<BulletType, BulletType> equipped_bullets;

  int EMP_VEL;
  int FROZEN_VEL;
  bool is_empd = false;
  bool is_frozen = false;
};