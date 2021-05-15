#include "Weapons/EMPBullet.hpp"

#include "Characters/Enemy.hpp"
EMPBullet::EMPBullet()
    : Bullet("assets/pngs/pacman_lasers_freeze.png",
             IDS::BULLET_ID + "_" + IDS::EMP_ID + "_" + to_string(rand())) {}

void EMPBullet::handle_collision() {
  auto collisions = CollisionEngine::getCollisions(ID);

  for (auto& item : collisions) {
    if (item->id.find(IDS::ENEMY_COLLIDER_ID) != -1) {
      auto temp = extractIntegerWords(item->id);
      if (temp.size() == 1) {
        temp[0]--;
        isActive = false;
        Enemy::get_enemies()[temp[0]]->emp();
      }
    }
  }
  Bullet::handle_collision();
}