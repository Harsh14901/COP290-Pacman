#include "Weapons/Weapon.hpp"

Weapon::Weapon(BulletType type, Character* bearer)
    : bullet_type(type), bearer(bearer) {}

void Weapon::fire(Direction d, int x, int y) {
  if (reload_animator.isActive()) {
    return;
  }
  if (bullet_count != 0) {
    BulletManager::shoot_bullet(bullet_type, d, x, y);
    bullet_count--;
  } else {
    reload();
  }
  printf("%s : %d/%d\n", bearer->ID.c_str(), bullet_count, ammo);
}

void Weapon::reload() {
  if (out_of_ammo()) {
    return;
  }
  reload_animator.start();
  auto mag_empty_count = magazine_cap - bullet_count;
  if (mag_empty_count < ammo) {
    ammo -= mag_empty_count;
    bullet_count = magazine_cap;
  } else {
    bullet_count += ammo;
    ammo = 0;
  }
}

void Weapon::handleEvent(SDL_Event& e) {
  if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
    switch (e.key.keysym.sym) {
      case SDLK_f:
        fire(bearer->_direction, bearer->mPosX, bearer->mPosY);
        break;
      case SDLK_r:
        reload();
        break;
      default:
        break;
    }
  }
}

bool Weapon::out_of_ammo() { return ammo == 0 && bullet_count == 0; }