#pragma once

#include <bits/stdc++.h>

#include "Constants.hpp"
#include "utils.hpp"
using namespace std;

enum Themes { MASTER, PACMAN, AVENGERS, MINECRAFT, COVID, JUNGLE,ALIENS,  STAR_WARS, TRANSFORMERS, MARIO, ROBOT, T_Last };

enum ThemeAssets {

  PACMAN_SPRITE,
  ENEMY_SPRITE,
  VENT_PNG,  // Note corresponding vent open and close are calculated
             // automatically
  COIN_PNG,
  COIN_SOUND,
  CHERRY_PNG,

  FREEZEBULLET_PNG,
  FREEZEBULLET_SOUND,

  EMP_PNG,
  EMP_SOUND,

  GRENADE_PNG,
  GRENADE_SOUND,

  WALLBUSTER_PNG,
  WALLBUSTER_SOUND,

  BOOST_PNG,
  INVISIBILITY_PNG,

  WALL_PNG,
  BACK,
  E_Last
};

// The main purpose of this class is to get back asset string from theme and key
class AssetManager {
 public:
  static void set_theme(Themes theme);
  static Themes get_theme();
  static string get_asset(ThemeAssets key);
  static void init(Themes startTheme);

 private:
  static Themes selected_theme;
  static unordered_map<Themes, string> theme_folders;
  static unordered_map<ThemeAssets, string> asset_files;
  static string asset_folder;

  static string get_asset(Themes theme, ThemeAssets asset);
};