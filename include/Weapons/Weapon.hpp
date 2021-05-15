#include "BulletManager.hpp"
#include "Characters/Character.hpp"
#include "Utils/Animator.hpp"

using namespace std;

class Weapon {
 public:
  int ammo = WeaponStats::AMMO;
  int magazine_cap = WeaponStats::MAG_CAP;
  int reload_time = WeaponStats::RELOAD_TIME;
  int muzzle_velocity = WeaponStats::MUZZLE_VEL;
  int bullet_count = magazine_cap;
  BulletType bullet_type;
  Weapon(BulletType type, Character* bearer);
  void reload();
  void fire(Direction d, int x, int y);
  bool out_of_ammo();
  void handleEvent(SDL_Event& e);

 private:
  Animator reload_animator = Animator(reload_time);
  Character* bearer;
};