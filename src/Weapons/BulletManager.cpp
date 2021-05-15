#include "Weapons/BulletManager.hpp"

vector<unique_ptr<Bullet>> BulletManager::active_bullets;
SDL_Renderer* BulletManager::renderer;

void BulletManager::init(SDL_Renderer* r) { renderer = r; }

void BulletManager::shoot_bullet(BulletType type, Direction d, int x, int y,
                                 bool broadcast) {
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
  if (broadcast) {
    broadcast_bullet(bullet_ptr, type);
  }
}

void BulletManager::render_bullets() {
  for (auto& p : active_bullets) {
    p.get()->render();
  }
}

void BulletManager::update_bullets() {
  recieve_bullets();
  active_bullets.erase(
      remove_if(active_bullets.begin(), active_bullets.end(),
                [](unique_ptr<Bullet>& p) { return !p.get()->is_active(); }),
      active_bullets.end());
  for (auto& p : active_bullets) {
    p.get()->move();
  }
}

void BulletManager::broadcast_bullet(Bullet* bullet, BulletType type) {
  Packet p;

  p.id = IDS::BULLET_MANAGER_ID;
  p.posX = bullet->mPosX;
  p.posY = bullet->mPosY;
  p.velX = int(bullet->_direction);
  p.velY = int(type);

  NetworkManager::queue_packet(p);
}

void BulletManager::recieve_bullets() {
  vector<Packet> packets;
  NetworkManager::get_packets(IDS::BULLET_MANAGER_ID, packets);

  for (auto& p : packets) {
    int x = p.posX;
    int y = p.posY;
    auto d = Direction(p.velX);
    auto t = BulletType(p.velY);
    shoot_bullet(t, d, x, y, false);
  }
}