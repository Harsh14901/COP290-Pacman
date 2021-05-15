#pragma once

#include "Bullet.hpp"
#include "Constants.hpp"
using namespace std;

class FreezeBullet : public Bullet {
 public:
  FreezeBullet();

 protected:
  void handle_collision();
};