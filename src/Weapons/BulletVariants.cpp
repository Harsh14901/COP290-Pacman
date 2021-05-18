#include "Weapons/BulletVariants.hpp"

#include "Grids/WallGrid.hpp"
extern AssetManager assetManager;
////////////////////////////////////////////////////////////////////////////////
/*
    FREEZEBULLET
*/
////////////////////////////////////////////////////////////////////////////////

FreezeBullet::FreezeBullet()
    : Bullet(assetManager.get_asset(ThemeAssets::FREEZEBULLET_PNG),
             IDS::BULLET_ID + "_" + IDS::FREEZEBULLET_ID + "_" +
                 to_string(rand())) {}

void FreezeBullet::init_targets() {
  add_target(IDS::PACMAN_COLLIDER_ID);
  Bullet::init_targets();
}

void FreezeBullet::target_hit(string target_id, Collider* collider) {
  if (target_id == IDS::PACMAN_COLLIDER_ID) {
    isActive = false;
  }
  Bullet::target_hit(target_id, collider);
}

////////////////////////////////////////////////////////////////////////////////
/*
    GRENADE
*/
////////////////////////////////////////////////////////////////////////////////
Grenade::Grenade()
    : Bullet(assetManager.get_asset(ThemeAssets::FREEZEBULLET_PNG), 0,
             IDS::BULLET_ID + "_" + IDS::GRENADE_ID + "_" + to_string(rand())) {
}

void Grenade::init_targets() {
  add_target(IDS::ENEMY_COLLIDER_ID);
  Bullet::init_targets();
}

void Grenade::target_hit(string target_id, Collider* collider) {
  if (target_id == IDS::ENEMY_COLLIDER_ID) {
    if (collider == nullptr) {
      return;
    }
    auto temp = extractIntegerWords(collider->id);
    if (temp.size() == 1) {
      isActive = false;
    }
  }
  Bullet::target_hit(target_id, collider);
}

////////////////////////////////////////////////////////////////////////////////
/*
    EMPBULLET
*/
////////////////////////////////////////////////////////////////////////////////

EMPBullet::EMPBullet()
    : Bullet(assetManager.get_asset(ThemeAssets::FREEZEBULLET_PNG),
             IDS::BULLET_ID + "_" + IDS::EMP_ID + "_" + to_string(rand())) {}

void EMPBullet::init_targets() {
  add_target(IDS::ENEMY_COLLIDER_ID);
  Bullet::init_targets();
}

void EMPBullet::target_hit(string target_id, Collider* collider) {
  if (target_id == IDS::ENEMY_COLLIDER_ID) {
    if (collider == nullptr) {
      return;
    }
    auto temp = extractIntegerWords(collider->id);
    if (temp.size() == 1) {
      isActive = false;
    }
  }
  Bullet::target_hit(target_id, collider);
}

////////////////////////////////////////////////////////////////////////////////
/*
    WALLBUSTER
*/
////////////////////////////////////////////////////////////////////////////////

WallBusterBullet::WallBusterBullet()
    : Bullet(assetManager.get_asset(ThemeAssets::FREEZEBULLET_PNG),
             IDS::BULLET_ID + "_" + IDS::WALLBUSTER_ID + "_" +
                 to_string(rand())) {}

void WallBusterBullet::target_hit(string target_id, Collider* collider) {
  if (target_id == IDS::WALL_COLLIDER_ID) {
    if (collider == nullptr) {
      return;
    }
    auto temp = extractIntegerWords(collider->id);
    if (temp.size() == 2) {
      WallGrid::getInstance()->unset_object(temp[0], temp[1]);
      isActive = false;
    }
  }
}