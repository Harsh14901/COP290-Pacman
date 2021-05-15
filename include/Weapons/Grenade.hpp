#pragma once

#include "Constants.hpp"
#include "Weapons/Bullet.hpp"
using namespace std;

class Grenade : public Bullet {
 public:
  Grenade();

 protected:
  void handle_collision();
};