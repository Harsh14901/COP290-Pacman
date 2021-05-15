#pragma once

#include "Constants.hpp"
#include "Network/NetworkManager.hpp"
#include "Network/Packet.hpp"
#include "Weapons/Bullet.hpp"
#include "Weapons/EMPBullet.hpp"
#include "Weapons/FreezeBullet.hpp"
#include "Weapons/Grenade.hpp"
#include "Weapons/WallBusterBullet.hpp"
#include "bits/stdc++.h"
using namespace std;

class BulletManager {
 public:
  static void shoot_bullet(BulletType type, Direction d, int x, int y,
                           bool broadcast = true);
  static void render_bullets();
  static void update_bullets();
  static void init(SDL_Renderer* renderer);

 private:
  static vector<unique_ptr<Bullet>> active_bullets;
  static SDL_Renderer* renderer;

  static void broadcast_bullet(Bullet* bullet, BulletType type);
  static void recieve_bullets();
};