#pragma once

#include <iostream>
#include <map>

#include "Utils/AssetManager.hpp"

using namespace std;

class PreferenceManager {
 public:
  static void initSettings();
  static bool MUSIC_ON;
  static bool SFX_ON;
  static Themes THEME;
  static int NUM_ENEMIES;

  //   First element -> primary weapon, second element -> secondary weapon.
  static pair<BulletType, BulletType> playerBullets;
  static pair<BulletType, BulletType> enemyBullets;
};