#include "Weapons/BulletVariants.hpp"

#include "Grids/WallGrid.hpp"
////////////////////////////////////////////////////////////////////////////////
/*
    FREEZEBULLET
*/
////////////////////////////////////////////////////////////////////////////////
FreezeBullet::FreezeBullet(string target)
    : Bullet(
          AssetManager::get_asset(ThemeAssets::FREEZEBULLET_PNG),
          IDS::BULLET_ID + "_" + IDS::FREEZEBULLET_ID + "_" + to_string(rand()),
          target) {}

////////////////////////////////////////////////////////////////////////////////
/*
    GRENADE
*/
////////////////////////////////////////////////////////////////////////////////
Grenade::Grenade(string target)
    : Bullet(AssetManager::get_asset(ThemeAssets::GRENADE_PNG), 0,
             IDS::BULLET_ID + "_" + IDS::GRENADE_ID + "_" + to_string(rand()),
             target) {}

////////////////////////////////////////////////////////////////////////////////
/*
    EMPBULLET
*/
////////////////////////////////////////////////////////////////////////////////
EMPBullet::EMPBullet(string target)
    : Bullet(AssetManager::get_asset(ThemeAssets::EMP_PNG),
             IDS::BULLET_ID + "_" + IDS::EMP_ID + "_" + to_string(rand()),
             target) {}

////////////////////////////////////////////////////////////////////////////////
/*
    WALLBUSTER
*/
////////////////////////////////////////////////////////////////////////////////
WallBusterBullet::WallBusterBullet()
    : Bullet(
          AssetManager::get_asset(ThemeAssets::WALLBUSTER_PNG),
          IDS::BULLET_ID + "_" + IDS::WALLBUSTER_ID + "_" + to_string(rand()),
          IDS::WALL_COLLIDER_ID) {}

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