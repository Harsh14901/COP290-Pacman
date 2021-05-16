#pragma once

#include "BulletManager.hpp"
#include "Characters/Character.hpp"
#include "Utils/Animator.hpp"

using namespace std;

class Weapon {
 public:
  int ammo;
  int magazine_cap;
  int reload_time;
  int burst_count;
  BulletType bullet_type;
  int bullet_count;
  Weapon();
  void init(BulletType type, Character* bearer, int burst_count);
  void reload();
  void fire(Direction d, int x, int y);
  bool out_of_ammo();
  bool is_reloading();
  void handleEvent(SDL_Event& e);

 private:
  Animator reload_animator;
  Character* bearer;
};

struct WeaponSet {
  Weapon primary_weapon;
  Weapon secondary_weapon;

  int active_weapon = 0;

  Weapon& get_active_weapon();
  void handleEvent(SDL_Event& e);
  string get_text();
};