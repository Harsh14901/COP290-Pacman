#include "Characters/Pacman.hpp"

extern vector<Enemy*> enemies;
extern bool is_server;

Pacman::Pacman() : Character(IDS::PACMAN_COLLIDER_ID) {}

void Pacman::init(SDL_Renderer* renderer) {
  _gDotTexture.setRenderer(renderer);
  _gDotTexture.loadFromFile("assets/pngs/pac-classic_c-toy.png");
  _gDotTexture.set_image_dimenstions(DOT_WIDTH, DOT_HEIGHT);
  CollisionEngine::register_collider(&mCollider);
  chompSound.init("assets/sounds/pacman_chomp.wav",false);
}

void Pacman::handleEvent(SDL_Event& e) {
  if (!is_server) {
    return;
  }
  Character::handleEvent(e);
}

void Pacman::render() {
  // Show the dot
  int mouth_fac = isMouthOpen() ? 0 : 2;
  SDL_Rect rect{138 * 4, 171 * ((2 - (int(_direction) % 2)) + mouth_fac), 138,
                171};
  _gDotTexture.render(mPosX, mPosY, &rect, 90 * (int(_direction) / 2));
}

void Pacman::incrementActivePoints(int inc){
  activePoints = min(activePoints+inc,100);
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
      if (temp.size() == 2) {
        if(enemies[temp[0]]->state!=EnemyState::WEAK){
          is_dead = true;
          return;
        }else{
          incrementActivePoints(50);
          enemies[temp[0]]->respawn();
        }
      }
      continue;
    }if(collisions[i]->id.find(IDS::FREEZEBULLET_ID)!=-1){
      cout << "Time to Freeze" << endl;
      // is_dead = true;
      i++;
      freezeAnimation.start();
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
  if(freezeAnimation.isActive()) {
    cout << "Freeze is active" << endl;
    return;
  }
  if (!is_server) {
    handle_packets();
  }
  handle_collision();

  Character::move();
}

int Pacman::get_coins_collected() { return coins; }
int Pacman::get_active_points() { return activePoints; }