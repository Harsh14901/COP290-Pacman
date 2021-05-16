#pragma once

#include "Constants.hpp"
#include "Weapons/Bullet.hpp"
using namespace std;

class FreezeBullet : public Bullet {
 public:
  FreezeBullet();

 protected:
  void init_targets() override;
  void target_hit(string target_id, Collider* collider) override;
};

class Grenade : public Bullet {
 public:
  Grenade();

 protected:
  void init_targets() override;
  void target_hit(string target_id, Collider* collider) override;
};

class EMPBullet : public Bullet {
 public:
  EMPBullet();

 protected:
  void init_targets() override;
  void target_hit(string target_id, Collider* collider) override;
};

class WallBusterBullet : public Bullet {
 public:
  WallBusterBullet();

 protected:
  void target_hit(string target_id, Collider* collider) override;
};