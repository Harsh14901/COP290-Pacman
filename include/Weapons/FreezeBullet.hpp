#pragma once

#include "BulletWeapon.hpp"
#include "Constants.hpp"
// #include "Characters/Pacman.hpp"
// #include "Characters/Enemy.hpp"
using namespace std;

class FreezeBullet : public BulletWeapon {
 public:
  FreezeBullet();
  void move() override;

 protected:
  void handle_collision() override;
};