#include "Utils/PreferenceManager.hpp"

bool PreferenceManager::MUSIC_ON;
bool PreferenceManager::SFX_ON;
Themes PreferenceManager::THEME;
int PreferenceManager::NUM_ENEMIES;
pair<BulletType, BulletType> PreferenceManager::playerBullets;
pair<BulletType, BulletType> PreferenceManager::enemyBullets;

void PreferenceManager::initSettings() {
  MUSIC_ON = true;
  SFX_ON = true;
  THEME = Themes::MINECRAFT;
  NUM_ENEMIES = 2;
  playerBullets = make_pair(BulletType::EMP, BulletType::GRENADE);
  enemyBullets = make_pair(BulletType::FREEZE, BulletType::WALLBUSTER);
  // playerBullets = make_pair(BulletType::FREEZE, BulletType::WALLBUSTER);
  // enemyBullets = make_pair(BulletType::EMP, BulletType::GRENADE);
}