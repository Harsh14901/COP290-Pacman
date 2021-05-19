#include "Characters/Pacman.hpp"

#include "Grids/PotionGrid.hpp"
#include "Grids/VentGrid.hpp"
#include "Utils/AssetManager.hpp"

extern bool is_server;
unique_ptr<Pacman> Pacman::_instance;

Pacman* Pacman::getInstance() {
  if (_instance.get() == nullptr) {
    _instance = make_unique<Pacman>();
  }
  return _instance.get();
}

void Pacman::clearInstance() { _instance = nullptr; }

Pacman::Pacman() : Character(IDS::PACMAN_COLLIDER_ID) {}

void Pacman::init(SDL_Renderer* renderer) {
  Base::init(renderer);
  chompSound.init(AssetManager::get_asset(ThemeAssets::COIN_SOUND), false);
  weaponSet.primary_weapon.init(BulletType::EMP, this, 1);
  weaponSet.secondary_weapon.init(BulletType::GRENADE, this, 1);

  boostAnimator.set_duration(200);

  for (int i = 0; i < TOTAL_PARTICLES; i++) {
    particles.push_back(make_unique<Particle>(mPosX, mPosY));
    particles.back().get()->init(renderer);
  }

  _gRenderer = renderer;
}

void Pacman::init_targets() {
  add_target(IDS::COIN_COLLIDER_ID);
  add_target(IDS::CHERRY_COLLIDER_ID);
  add_target(IDS::ENEMY_COLLIDER_ID);
  add_target(IDS::FREEZEBULLET_ID);
  add_target(IDS::BOOST_ID);
  add_target(IDS::INVISIBILITY_ID);
  Character::init_targets();
}

void Pacman::handleEvent(SDL_Event& e) {
  if (!is_server) {
    return;
  }
  if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
    if (e.key.keysym.sym == SDLK_v) {
      VentGrid::getInstance()->handleOpening(mPosX / 32, mPosY / 32);
    }
  }
  weaponSet.handleEvent(e);
  Character::handleEvent(e);
}

void Pacman::render() {
  // Show the dot
  int mouth_fac = isMouthOpen() ? 0 : 2;
  SDL_Rect rect{0, 171 * ((1 - (int(_direction) % 2)) + mouth_fac), 162, 171};

  _gDotTexture.setAlpha(getInvisibleAlphaValue());
  _gDotTexture.render(mPosX, mPosY, &rect, 90 * (int(_direction) / 2));
  renderParticles();
}

// Note: this is an impure function
int Pacman::getInvisibleAlphaValue() {
  if (!invisibleAnimator.isActive()) {
    if (is_server) is_invisible = false;
    return is_server ? 255 : is_invisible ? 0 : 255;
  }

  if (invisibleAnimator.animation_progress() < 0.2) {
    double val = invisibleAnimator.getAnimationProgressInCurve(
        AnimationCurve::IncreasingFreqSine, 3 / (0.2), 0);
    return val > 0 ? 255 : is_server ? 100 : 50;
  }
  if (invisibleAnimator.animation_progress() > 0.8) {
    double val = invisibleAnimator.getAnimationProgressInCurve(
        AnimationCurve::IncreasingFreqSine, 3 / (0.2), 1 - 0.2);
    return val > 0 ? 255 : is_server ? 100 : 50;
  }
  return is_server ? 100 : 0;
}

void Pacman::incrementActivePoints(int inc) {
  activePoints = max(0, min(activePoints + inc, 100));
}

void Pacman::target_hit(string target_id, Collider* collider) {
  if (collider != nullptr) {
    auto temp = extractIntegerWords(collider->id);
    if (target_id == IDS::COIN_COLLIDER_ID) {
      if (temp.size() == 2) {
        collect_coins(temp[0], temp[1]);
      }
    }
    if (target_id == IDS::CHERRY_COLLIDER_ID) {
      if (temp.size() == 2) {
        collect_cherries(temp[0], temp[1]);
      }
    }
    if (target_id == IDS::ENEMY_COLLIDER_ID) {
      if (temp.size() == 1) {
        enemy_collision(temp[0] - 1);
      }
    }
    if (target_id == IDS::BOOST_ID) {
      if (temp.size() == 2) {
        boost();
        BoostGrid::getInstance()->unset_object(temp[0], temp[1]);
      }
    }
    if (target_id == IDS::INVISIBILITY_ID) {
      if (temp.size() == 2) {
        make_invisible();
        InvisibilityGrid::getInstance()->unset_object(temp[0], temp[1]);
      }
    }
  }
  if (target_id == IDS::FREEZEBULLET_ID) {
    freeze();
  }
  Character::target_hit(target_id, collider);
}

void Pacman::collect_coins(int i, int j) {
  auto coinGrid = CoinGrid::getInstance();

  coins++;
  incrementActivePoints(1);

  chompSound.play();
  gulp_animator.start();

  coinGrid->unset_object(i, j);
}
void Pacman::collect_cherries(int i, int j) {
  auto cherryGrid = CherryGrid::getInstance();
  auto enemies = Enemy::get_enemies();

  cherries++;
  incrementActivePoints(5);
  cherryGrid->unset_object(i, j);

  for (auto& x : enemies) {
    x->setState(EnemyState::WEAK);
  }
}

void Pacman::enemy_collision(int num) {
  auto enemies = Enemy::get_enemies();

  if (enemies[num]->state != EnemyState::WEAK && !is_invisible) {
    is_dead = true;
  } else {
    incrementActivePoints(50);
    enemies[num]->respawn();
  }
}

void Pacman::freeze() {
  cout << "Time to Freeze" << endl;
  incrementActivePoints(-20);
  freezeAnimation.start();
}

bool Pacman::isMouthOpen() {
  if (gulp_animator.isActive()) {
    int cf = gulp_animator.get_current_frame();
    if (cf == 1 || cf == 2) {
      return false;
    }
  }
  return true;
}

void Pacman::boost() { boostAnimator.start(); }

void Pacman::make_invisible() {
  if (!is_invisible && get_active_points() >= 50) {
    is_invisible = true;
    invisibleAnimator.start();
    incrementActivePoints(-50);
  }
}

void Pacman::check_boost() {
  auto swap_vels = [&]() {
    auto temp = BOOST_VEL;
    BOOST_VEL = MAX_VEL;
    MAX_VEL = temp;
  };
  if (!WallGrid::getInstance()->fits_in_cell(mPosX, mPosY)) {
    return;
  }
  if (boostAnimator.isActive() && !is_boosted) {
    swap_vels();
    is_boosted = true;
    change_direction(_direction);
    cout << "Boosted velocity: " << MAX_VEL << endl;
  }
  if (!boostAnimator.isActive() && is_boosted) {
    swap_vels();
    is_boosted = false;
    change_direction(_direction);

    cout << "Restoring velocity: " << MAX_VEL << endl;
  }
}
void Pacman::move() {
  if (freezeAnimation.isActive()) {
    return;
  }

  if (!is_server) {
    handle_packets();
  }
  handle_collision();
  check_boost();
  Character::move();

  if (VentGrid::getInstance()->canTeleport()) {
    auto newPt = VentGrid::getInstance()->getTeleportLocation();
    mPosX = newPt.first;
    mPosY = newPt.second;
    cout << "x,y are" << mPosX << "," << mPosY << endl;
    mVelX = 0;
    mVelY = 0;
    broadcast_coordinates();
  }
}

int Pacman::get_coins_collected() { return coins; }
int Pacman::get_active_points() { return activePoints; }
string Pacman::get_weapon_text() { return weaponSet.get_text(); }
void Pacman::handle_packets() {
  vector<Packet> packets;
  NetworkManager::get_packets(ID, packets);

  for (auto& p : packets) {
    mPosX = p.posX;
    mPosY = p.posY;
    mVelX = p.velX;
    mVelY = p.velY;
    auto data = convert_string_to_map(p.data);
    // cout << "See this "<< is_server<< " " << data["is_invisible"] << endl;
    _direction = Direction(stoi(data["direction"]));
    if (!is_server) is_invisible = data["is_invisible"] == "1";
    if (data["is_boosted"] == "1") {
      boost();
    }
  }
}

void Pacman::broadcast_coordinates() {
  Packet p;
  p.id = ID;
  p.posX = mPosX;
  p.posY = mPosY;
  p.velX = mVelX;
  p.velY = mVelY;
  unordered_map<string, string> data;
  data.insert({"direction", to_string(int(_direction))});
  data.insert({"is_invisible", to_string(is_invisible)});
  data.insert({"is_boosted", to_string(is_boosted)});
  p.data = map_to_string(data);
  // cout << "data is " << p.data << endl;
  NetworkManager::queue_packet(p);
}

void Pacman::renderParticles() {
  // Go through particles
  for (int i = 0; i < TOTAL_PARTICLES; ++i) {
    // Delete and replace dead particles
    if (particles[i]->isDead()) {
      particles[i] = make_unique<Particle>(mPosX, mPosY);
      particles[i]->init(_gRenderer);
    }
  }

  // Show particles only if boost is active
  if (!is_boosted) return;
  for (int i = 0; i < TOTAL_PARTICLES; ++i) {
    particles[i]->update(mPosX, mPosY);
    particles[i]->render();
  }
}