#include "Constants.hpp"
#include "Weapons/Bullet.hpp"
#include "Weapons/FreezeBullet.hpp"
#include "bits/stdc++.h"
using namespace std;

class BulletManager {
 public:
  static void shoot_bullet(BulletType type, Direction d, int x, int y);
  static void render_bullets();
  static void update_bullets();
  static void init(SDL_Renderer* renderer);

 private:
  static vector<unique_ptr<Bullet>> active_bullets;
  static SDL_Renderer* renderer;
};