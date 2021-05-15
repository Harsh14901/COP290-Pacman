#include "Weapons/BulletManager.hpp"

vector<unique_ptr<Bullet>> BulletManager::active_bullets;
SDL_Renderer* BulletManager::renderer;

void BulletManager::init(SDL_Renderer* r) { renderer = r; }

void BulletManager::shoot_bullet(BulletType type, Direction d, int x, int y) {
  switch (type) {
    case BulletType::FREEZE:
      active_bullets.push_back(make_unique<FreezeBullet>());

      break;

    default:
      break;
  }
  auto bullet_ptr = active_bullets.back().get();
  bullet_ptr->init(renderer);
  bullet_ptr->shoot(d, x, y);
}

void BulletManager::render_bullets() {
  for (auto& p : active_bullets) {
    p.get()->render();
  }
}

void BulletManager::update_bullets() {
  active_bullets.erase(
      remove_if(active_bullets.begin(), active_bullets.end(),
                [](unique_ptr<Bullet>& p) { return !p.get()->is_active(); }),
      active_bullets.end());
  for (auto& p : active_bullets) {
    p.get()->move();
  }
}