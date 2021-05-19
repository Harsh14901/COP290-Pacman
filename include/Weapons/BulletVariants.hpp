#pragma once

#include "Constants.hpp"
#include "Utils/AssetManager.hpp"
#include "Weapons/Bullet.hpp"
using namespace std;

class FreezeBullet : public Bullet {
 public:
  FreezeBullet(string target);
};

class Grenade : public Bullet {
 public:
  Grenade(string target);
};

class EMPBullet : public Bullet {
 public:
  EMPBullet(string target);
};

class WallBusterBullet : public Bullet {
 public:
  WallBusterBullet();

 protected:
  void target_hit(string target_id, Collider* collider) override;
};