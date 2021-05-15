#include "BulletManager.hpp"
#include "Characters/Character.hpp"
#include "Utils/Animator.hpp"

using namespace std;

class Weapon {
 public:
  int ammo = WeaponStats::AMMO;
  int magazine_cap = WeaponStats::MAG_CAP;
  int reload_time = WeaponStats::RELOAD_TIME;
  int burst_count = WeaponStats::BURST_COUNT;
  BulletType bullet_type = BulletType::FREEZE;
  int bullet_count = magazine_cap;
  Weapon();
  void init(BulletType type, Character* bearer, int burst_count);
  void reload();
  void fire(Direction d, int x, int y);
  bool out_of_ammo();
  void handleEvent(SDL_Event& e);

 private:
  Animator reload_animator = Animator(reload_time);
  Character* bearer;
};