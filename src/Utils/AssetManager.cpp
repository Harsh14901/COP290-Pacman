#include "Utils/AssetManager.hpp"

string AssetManager::asset_folder;
Themes AssetManager::selected_theme = Themes::MASTER;
unordered_map<Themes, string> AssetManager::theme_folders;
unordered_map<ThemeAssets, string> AssetManager::asset_files;

void AssetManager::init(Themes startTheme) {
  selected_theme = startTheme;
  asset_folder = "assets/themes/";
  theme_folders = unordered_map<Themes, string>(
      {{MASTER, "master/"}, {PACMAN, "pacman/"}, {AVENGERS, "avengers/"}});
  asset_files = unordered_map<ThemeAssets, string>({
      {PACMAN_SPRITE, "player.png"},
      {ENEMY_SPRITE, "enemy.png"},
      {VENT_PNG, "vent.png"},
      {COIN_PNG, "coin.png"},
      {CHERRY_PNG, "cherry.png"},
      {COIN_SOUND, "coin_collect.mp3"},

      {EMP_PNG, "freeze_bullet.png"},
      {EMP_SOUND, "freeze_bullet.wav"},
      {GRENADE_PNG, "freeze_bullet.png"},
      {GRENADE_SOUND, "freeze_bullet.wav"},
      {WALLBUSTER_PNG, "freeze_bullet.png"},
      {WALLBUSTER_SOUND, "freeze_bullet.wav"},
      {FREEZEBULLET_PNG, "freeze_bullet.png"},
      {FREEZEBULLET_SOUND, "freeze_bullet.wav"},
      {BOOST_PNG, "boost_potion.png"},
      {INVISIBILITY_PNG, "invisibility_potion.png"},
  });
}
void AssetManager::set_theme(Themes theme) { selected_theme = theme; }

Themes AssetManager::get_theme() { return selected_theme; }

string AssetManager::get_asset(ThemeAssets key) {
  if (theme_folders.empty() && asset_files.empty()) {
    init(MASTER);
  }
  auto asset = get_asset(selected_theme, key);
  if (!fileExists(asset)) {
    asset = get_asset(MASTER, key);
  }

  if (!fileExists(asset)) {
    fatalError("Asset could not be found: " + asset);
  }

  return asset;
}

string AssetManager::get_asset(Themes theme, ThemeAssets asset) {
  return asset_folder + theme_folders[theme] + asset_files[asset];
}