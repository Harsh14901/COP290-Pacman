#pragma once

#include "Constants.hpp"
#include "Weapons/Bullet.hpp"
using namespace std;

class WallBusterBullet : public Bullet {
 public:
  WallBusterBullet();

 protected:
  void handle_collision();
};