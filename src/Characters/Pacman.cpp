#include "Characters/Pacman.hpp"

#include "Grids/VentGrid.hpp"

extern vector<Enemy*> enemies;
extern bool is_server;

Pacman::Pacman() : Character(IDS::PACMAN_COLLIDER_ID) {}

void Pacman::init(SDL_Renderer* renderer) {
  Base::init(renderer);
  chompSound.init("assets/sounds/pacman_chomp.wav", false);
}

void Pacman::handleEvent(SDL_Event& e) {
  if (!is_server) {
    return;
  }
  if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
    if (e.key.keysym.sym == SDLK_q) {
      if (!is_invisible && get_active_points() >= 50) {
        is_invisible = true;
        invisibleAnimator.start();
        incrementActivePoints(-50);
      }
    } else if (e.key.keysym.sym == SDLK_v) {
      VentGrid::getInstance()->handleOpening(mPosX / 32, mPosY / 32);
    }
  }
  Character::handleEvent(e);
}

void Pacman::render() {
  // Show the dot
  int mouth_fac = isMouthOpen() ? 0 : 2;
  SDL_Rect rect{138 * 4, 171 * ((2 - (int(_direction) % 2)) + mouth_fac), 138,
                171};

  _gDotTexture.setAlpha(getInvisibleAlphaValue());
  _gDotTexture.render(mPosX, mPosY, &rect, 90 * (int(_direction) / 2));
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

void Pacman::handle_collision() {
  auto collisions = CollisionEngine::getCollisions(IDS::PACMAN_COLLIDER_ID);
  int i = 0;
  // cout << "Inside pacman collision" << endl;

  auto coinGrid = CoinGrid::getInstance();
  auto cherryGrid = CherryGrid::getInstance();
  auto enemies = Enemy::get_enemies();

  // if(!collisions.empty()){
  while (i < collisions.size()) {
    if (collisions[i]->id.find(IDS::COIN_COLLIDER_ID) != -1) {
      // Coin Collected
      // cout << "Coin Collected" << endl;
      i++;
      coins++;
      incrementActivePoints(1);
      chompSound.play();
      auto temp = extractIntegerWords(collisions[i - 1]->id);
      if (temp.size() == 2) {
        coinGrid->unset_object(temp[0], temp[1]);
      }
      gulp_animator.start();
      continue;
    }
    if (collisions[i]->id.find(IDS::CHERRY_COLLIDER_ID) != -1) {
      // cout << "Cherry Collected" << endl;
      i++;
      cherries++;
      incrementActivePoints(5);
      auto temp = extractIntegerWords(collisions[i - 1]->id);
      if (temp.size() == 2) {
        cherryGrid->unset_object(temp[0], temp[1]);
      }
      for (auto& x : enemies) {
        x->setState(EnemyState::WEAK);
      }
      continue;
    }
    if (collisions[i]->id.find(IDS::ENEMY_COLLIDER_ID) != -1) {
      // Assert: Game Over
      i++;
      auto temp = extractIntegerWords(collisions[i - 1]->id);
      if (temp.size() == 1) {
        temp[0]--;
        if (enemies[temp[0]]->state != EnemyState::WEAK && !is_invisible) {
          is_dead = true;
          return;
        } else {
          incrementActivePoints(50);
          enemies[temp[0]]->respawn();
        }
      }
      continue;
    }
    if (collisions[i]->id.find(IDS::FREEZEBULLET_ID) != -1) {
      cout << "Time to Freeze" << endl;
      // is_dead = true;
      i++;
      incrementActivePoints(-20);
      freezeAnimation.start();
      continue;
    }
    if (collisions[i]->id.find(IDS::VENT_COLLIDER_ID) != -1) {
      i++;
      continue;
    }

    // cout << "Collision of pacman with " << collisions[i]->id << endl;
    Character::handle_collision();
    break;
  }
  // cout << "Outside pacman collision" << endl;
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

void Pacman::move() {
  if (freezeAnimation.isActive()) {
    cout << "Freeze is active" << endl;
    return;
  }
  if (!is_server) {
    handle_packets();
  }
  handle_collision();

  Character::move();

  if (VentGrid::getInstance()->canTeleport()) {
    auto newPt = VentGrid::getInstance()->getTeleportLocation();
    mPosX = newPt.first;
    mPosY = newPt.second;
    cout << "x,y are" << mPosX << "," << mPosY << endl;
    mVelX = 0;
    mVelY = 0;
  }

  broadcast_coordinates();
}

int Pacman::get_coins_collected() { return coins; }
int Pacman::get_active_points() { return activePoints; }

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
  }
}

void Pacman::broadcast_coordinates() {
  Packet p;
  p.id = ID;
  p.posX = mPosX;
  p.posY = mPosY;
  p.velX = mVelX;
  p.velY = mVelY;
  map<string, string> data;
  data.insert({"direction", to_string(int(_direction))});
  data.insert({"is_invisible", to_string(is_invisible)});

  p.data = map_to_string(data);
  // cout << "data is " << p.data << endl;
  NetworkManager::queue_packet(p);
}
