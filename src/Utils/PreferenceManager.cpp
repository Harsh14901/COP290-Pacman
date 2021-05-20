#include "Utils/PreferenceManager.hpp"

#include "Network/NetworkManager.hpp"
#include "utils.hpp"

bool PreferenceManager::MUSIC_ON;
bool PreferenceManager::SFX_ON;
Themes PreferenceManager::THEME;
int PreferenceManager::NUM_ENEMIES;
pair<BulletType, BulletType> PreferenceManager::playerBullets;
pair<BulletType, BulletType> PreferenceManager::enemyBullets;

void PreferenceManager::broadcast() {
  Packet p;
  p.id = IDS::PREF_MANAGER_ID;
  unordered_map<string, string> data;

  data["num_enemies"] = to_string(PreferenceManager::NUM_ENEMIES);
  data["theme"] = to_string(int(PreferenceManager::THEME));
  p.data = map_to_string(data);
  NetworkManager::queue_packet(p);
  NetworkManager::send_all();
}

void PreferenceManager::recv_settings() {
  NetworkManager::recv_all();
  vector<Packet> packets;
  NetworkManager::get_packets(IDS::PREF_MANAGER_ID, packets);

  for (auto& p : packets) {
    auto data = convert_string_to_map(p.data);
    NUM_ENEMIES = stoi(data["num_enemies"]);
    THEME = Themes(stoi(data["theme"]));
  }
}
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