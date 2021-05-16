#include "Weapons/Weapon.hpp"

Weapon::Weapon() {}

void Weapon::init(BulletType type, Character* bearer, int burst_count) {
  this->bullet_type = type;
  this->bearer = bearer;
  this->burst_count = burst_count;
  switch (type) {
    case BulletType::FREEZE:
      ammo = WeaponStats::Freeze::AMMO;
      magazine_cap = WeaponStats::Freeze::MAG_CAP;
      reload_time = WeaponStats::Freeze::RELOAD_TIME;
      break;

    case BulletType::WALLBUSTER:
      ammo = WeaponStats::WallBuster::AMMO;
      magazine_cap = WeaponStats::WallBuster::MAG_CAP;
      reload_time = WeaponStats::WallBuster::RELOAD_TIME;
      break;

    case BulletType::EMP:
      ammo = WeaponStats::EMP::AMMO;
      magazine_cap = WeaponStats::EMP::MAG_CAP;
      reload_time = WeaponStats::EMP::RELOAD_TIME;
      break;

    case BulletType::GRENADE:
      ammo = WeaponStats::GRENADE::AMMO;
      magazine_cap = WeaponStats::GRENADE::MAG_CAP;
      reload_time = WeaponStats::GRENADE::RELOAD_TIME;
      break;

    default:
      break;
  }
  bullet_count = magazine_cap;
  reload_animator.set_duration(reload_time);
}

void Weapon::fire(Direction d, int x, int y) {
  if (reload_animator.isActive()) {
    return;
  }
  int i = burst_count;
  while (i > 0 && bullet_count > 0) {
    BulletManager::shoot_bullet(bullet_type, d, x, y);
    bullet_count--;
    i--;
  }
  if (bullet_count == 0) {
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
bool Weapon::is_reloading() { return reload_animator.isActive(); }

Weapon& WeaponSet::get_active_weapon() {
  return (active_weapon == 0) ? primary_weapon : secondary_weapon;
}

void WeaponSet::handleEvent(SDL_Event& e) {
  if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
    switch (e.key.keysym.sym) {
      case SDLK_1:
        active_weapon = 0;
        cout << "active weapon: " << int(primary_weapon.bullet_type) << endl;
        break;
      case SDLK_2:
        active_weapon = 1;
        cout << "active weapon: " << int(secondary_weapon.bullet_type) << endl;

        break;
      default:
        break;
    }
  }
  get_active_weapon().handleEvent(e);
}

string WeaponSet::get_text() {
  auto& weapon = get_active_weapon();
  string text;
  switch (weapon.bullet_type) {
    case BulletType::FREEZE:
      text = "Freeze Gun";
      break;

    case BulletType::WALLBUSTER:
      text = "Wall Buster";
      break;

    case BulletType::EMP:
      text = "EMP";
      break;

    case BulletType::GRENADE:
      text = "Land Mine";
      break;

    default:
      break;
  }
  text += " - ";
  if (weapon.out_of_ammo()) {
    text += "No Ammo";
  } else if (weapon.is_reloading()) {
    text += "Reloading ..";
  } else {
    text += to_string(weapon.bullet_count) + "/" + to_string(weapon.ammo);
  }
  return text;
}
