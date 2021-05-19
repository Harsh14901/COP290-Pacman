#pragma once

#include "Constants.hpp"
#include "Network/NetworkManager.hpp"
#include "Network/Packet.hpp"
#include "Utils/AudioAsset.hpp"
#include "Weapons/Bullet.hpp"
#include "Weapons/BulletVariants.hpp"
#include "bits/stdc++.h"
using namespace std;

class BulletManager {
 public:
  static void shoot_bullet(BulletType type, Direction d, int x, int y,
                           string target, bool broadcast = true);
  static void render_bullets();
  static void update_bullets();
  static void init(SDL_Renderer* renderer);
  static void clear_all();

 private:
  static vector<unique_ptr<Bullet>> active_bullets;
  static SDL_Renderer* renderer;

  static void broadcast_bullet(Bullet* bullet, BulletType type);
  static void recieve_bullets();

  static unordered_map<BulletType, unique_ptr<AudioAsset>> sounds;
};