#include "Weapons/FreezeBullet.hpp"

#include "Characters/Pacman.hpp"

extern bool is_server;

FreezeBullet::FreezeBullet()
    : Bullet("assets/pngs/pacman_lasers_freeze.png",
             IDS::BULLET_ID + "_" + IDS::FREEZEBULLET_ID + "_" +
                 to_string(rand())) {}

void FreezeBullet::handle_collision() {
  auto collisions = CollisionEngine::getCollisions(ID);

  for (auto& item : collisions) {
    if (item->id.find(IDS::WALL_COLLIDER_ID) != -1) {
      auto temp = extractIntegerWords(item->id);
      if (temp.size() == 2) {
        WallGrid::getInstance()->unset_object(temp[0], temp[1]);
      }
    }
  }
  Bullet::handle_collision();
}
