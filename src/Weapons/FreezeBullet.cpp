#include "Weapons/FreezeBullet.hpp"

#include "Characters/Pacman.hpp"

extern bool is_server;

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
  BulletWeapon::handle_collision();
}

FreezeBullet::FreezeBullet()
    : BulletWeapon("assets/pngs/pacman_lasers_freeze.png",
                   IDS::FREEZEBULLET_ID) {}

void FreezeBullet::move() {
  if (is_server) {
    // cout << "Receiving coords" << endl;
    // BulletWeapon::handle_packets();
  }
  handle_collision();
  if (!isLaunched) return;
  // cout << "Prior to update:" << this->mPosX << " , " << this->mPosY << endl;
  mPosX += mVelX;
  mPosY += mVelY;
  // cout << "Updating bullet: " << this->mPosX << " , " << this->mPosY << endl;

  mCollider.setX(mPosX);
  mCollider.setY(mPosY);

  if (!is_server) {
    // cout << "Broadcasting Coordis" << endl;
    // BulletWeapon::broadcast_coordinates();
  }
}