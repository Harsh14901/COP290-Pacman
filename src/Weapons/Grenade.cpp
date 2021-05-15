#include "Weapons/Grenade.hpp"

#include "Characters/Enemy.hpp"

Grenade::Grenade()
    : Bullet("assets/pngs/pacman_lasers_freeze.png", 0,
             IDS::BULLET_ID + "_" + IDS::WALLBUSTER_ID + "_" +
                 to_string(rand())) {}

void Grenade::handle_collision() {
  auto collisions = CollisionEngine::getCollisions(ID);

  for (auto& item : collisions) {
    if (item->id.find(IDS::ENEMY_COLLIDER_ID) != -1) {
      auto temp = extractIntegerWords(item->id);
      if (temp.size() == 1) {
        temp[0]--;
        Enemy::get_enemies()[temp[0]]->respawn();
      }
    }
  }
  Bullet::handle_collision();
}