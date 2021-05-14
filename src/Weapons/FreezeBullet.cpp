#include "Weapons/FreezeBullet.hpp"

#include "Characters/Pacman.hpp"

extern bool is_server;

void FreezeBullet::checkImpact() {
  auto collisions = CollisionEngine::getCollisions(ColliderID);

  for (auto& item : collisions) {
    if (item->id.find(IDS::WALL_COLLIDER_ID) != -1) {
      auto temp = extractIntegerWords(item->id);
      if (temp.size() == 2) {
        // auto p = WallGrid::getInstance()->get_canvas_point(
        //     SDL_Point{temp[0], temp[1]});
        // printf("Collided with wall : (%d, %d)\n", p.x, p.y);
        WallGrid::getInstance()->unset_object(temp[0], temp[1]);
        isLaunched = false;
      }
    }
  }
}

FreezeBullet::FreezeBullet(int vel)
    : BulletWeapon("assets/pngs/pacman_lasers_freeze.png", vel,
                   IDS::FREEZEBULLET_ID) {}

void FreezeBullet::update() {
  if (is_server) {
    // cout << "Receiving coords" << endl;
    BulletWeapon::receive_coordinates();
  }
  checkImpact();
  if (!isLaunched) return;
  // cout << "Prior to update:" << this->x << " , " << this->y << endl;
  x += velX;
  y += velY;
  // cout << "Updating bullet: " << this->x << " , " << this->y << endl;

  mCollider.setX(x);
  mCollider.setY(y);

  if (!is_server) {
    // cout << "Broadcasting Coordis" << endl;
    BulletWeapon::broadcast_coordinates();
  }
}