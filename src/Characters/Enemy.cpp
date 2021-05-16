#include "Characters/Enemy.hpp"

extern GhostManager ghostManager;
extern bool is_server;
int counter = 0;
vector<unique_ptr<Enemy>> Enemy::enemies;
int Enemy::active_index = 0;

void Enemy::make_enemies(int n) {
  enemies.resize(n);
  for (int i = 0; i < n; i++) {
    enemies[i] = make_unique<Enemy>(i);
  }
}

vector<Enemy*> Enemy::get_enemies() {
  vector<Enemy*> enemy_ptrs;
  for (int i = 0; i < enemies.size(); i++) {
    enemy_ptrs.push_back(enemies[i].get());
  }
  return enemy_ptrs;
}

Enemy::Enemy(int type)
    : Character(IDS::ENEMY_COLLIDER_ID + "_" + to_string(type + 1)) {
  this->type = type + 1;
  this->id = counter++;
}

Enemy* Enemy::get_active_enemy() { return enemies[active_index].get(); }

int Enemy::get_active_id() {
  if (enemies.empty()) {
    return -1;
  }
  if (active_index >= enemies.size()) {
    active_index = 0;
  }
  return enemies[active_index]->id;
}

void Enemy::switch_active_id() {
  if (is_server) return;

  if (active_index == enemies.size() - 1) {
    active_index = 0;
  } else {
    active_index++;
  }
}
void Enemy::handleEvent(SDL_Event& e) {
  if (is_server) {
    return;
  }
  if (id != get_active_id()) {
    return;
  }
  weaponSet.handleEvent(e);
  Character::handleEvent(e);
}

void Enemy::init(SDL_Renderer* renderer) {
  Character::init(renderer);
  spawnAnimator.set_duration(100 + 100 * type);
  AIEngine.init(WIDTH, HEIGHT, type);
  weaponSet.primary_weapon.init(BulletType::WALLBUSTER, this, 1);
  weaponSet.secondary_weapon.init(BulletType::FREEZE, this, 1);
}

void Enemy::init_targets() {
  add_target(IDS::GRENADE_ID);
  add_target(IDS::EMP_ID);
  Character::init_targets();
}

void Enemy::target_hit(string target_id, Collider* collider) {
  if (target_id == IDS::GRENADE_ID) {
    respawn();
  }
  if (target_id == IDS::EMP_ID) {
    emp();
  }
  Character::target_hit(target_id, collider);
}

string Enemy::get_weapon_text() { return weaponSet.get_text(); }

void Enemy::emp() { empAnimation.start(); }
void Enemy::render() {
  // Show the dot
  int typeValue = getEnemyColor();
  SDL_Rect rect{138 * (2 + int(_direction) % 2), 171 * typeValue, 138, 171};
  _gDotTexture.render(mPosX, mPosY, &rect, 90 * (int(_direction) / 2));
}

int Enemy::getEnemyColor() {
  if (weak_state_animator.isActive()) {
    if (weak_state_animator.animation_progress() > 0.75) {
      if (int(weak_state_animator.animation_progress() * 40) % 2 == 1) {
        return 4;
      }
    }
    return 5;
  }
  return type;
}

void Enemy::randomize_direction() {
  unordered_set<Direction> available_directions;
  for (int i = 0; i < 4; i++) {
    auto d = Direction(i);

    if (WallGrid::getInstance()->can_move(mPosX + WIDTH / 2, mPosY + HEIGHT / 2,
                                          d)) {
      // printf("Direction %d is available\n", i);
      available_directions.insert(d);
    }
  }
  auto directionToErase = _direction == Direction::LEFT    ? Direction::RIGHT
                          : _direction == Direction::RIGHT ? Direction::LEFT
                          : _direction == Direction::UP    ? Direction::DOWN
                                                           : Direction::UP;
  available_directions.erase(directionToErase);

  if (available_directions.size() != 0) {
    int selection = rand() % available_directions.size();
    int i = 0;
    for (auto& it : available_directions) {
      if (selection == i) {
        change_direction(it);
        break;
      }
      i++;
    }
  } else {
    // No option but to change direction
    if (mPosX % 32 == 0 && mPosY % 32 == 0) change_direction(directionToErase);
  }
}

void Enemy::check_emp() {
  auto swap_vels = [&]() {
    auto temp = EMP_VEL;
    EMP_VEL = MAX_VEL;
    MAX_VEL = temp;
  };
  if (empAnimation.isActive() && !is_empd) {
    swap_vels();
    is_empd = true;
  }
  if (!empAnimation.isActive() && is_empd) {
    swap_vels();
    is_empd = false;
  }
}
void Enemy::move() {
  check_emp();
  if (state == EnemyState::WEAK && !weak_state_animator.isActive()) {
    state = EnemyState::NORMAL;
  }

  AIEngine.update(mPosX, mPosY, _direction, state);

  if (is_server) {
    handle_packets();
    handle_collision();
    if (spawnAnimator.isActive()) {
      if (mVelX == 0) {
        if (mPosX == ghostManager.ghostZones[2].first * 32)
          change_direction(Direction::RIGHT);
        else
          change_direction(Direction::LEFT);
      }
      if (mPosX == ghostManager.ghostZones[2].first * 32) {
        change_direction(Direction::RIGHT);
      } else if (mPosX == ghostManager.ghostZones[5].first * 32) {
        change_direction(Direction::LEFT);
      } else {
        if (spawnAnimator.animation_progress() > 0.85 &&
            (mPosX != ghostManager.ghostZones[3].first * 32 &&
             mPosX != ghostManager.ghostZones[4].first * 32) &&
            (mPosX != ghostManager.ghostZones[2].first * 32 &&
             mPosX != ghostManager.ghostZones[5].first * 32)) {
          change_direction(Direction::UP);
        }
      }

    } else if (!is_two_player) {
      // randomize_direction();
      // AIEngine.updateDirection();
      change_direction(AIEngine.updateDirection());
    }
    Character::move();
  } else {
    handle_collision();
    if (get_active_id() != id) {
      // randomize_direction();
      change_direction(AIEngine.updateDirection());
    }
    Character::move();
  }
}

void Enemy::setState(EnemyState st) {
  state = st;
  weak_state_animator.start();
}

void Enemy::respawn() {
  auto pt = ghostManager.ghostZones[2 + (rand() % 4)];

  Character::place(
      WallGrid::getInstance()->getRenderPointFromCoordi(pt.first, pt.second));
  state = EnemyState::NORMAL;
  spawnAnimator.start();
}

void Enemy::handle_packets() {
  vector<Packet> packets;
  NetworkManager::get_packets(ID, packets);

  for (auto& p : packets) {
    mPosX = p.posX;
    mPosY = p.posY;
    mVelX = p.velX;
    mVelY = p.velY;
    auto data = convert_string_to_map(p.data);
    _direction = Direction(stoi(data["direction"]));
    if (data["empd"] == "1") {
      emp();
    }
  }
}

void Enemy::broadcast_coordinates() {
  Packet p;
  p.id = ID;
  p.posX = mPosX;
  p.posY = mPosY;
  p.velX = mVelX;
  p.velY = mVelY;

  unordered_map<string, string> data;
  data.insert({"direction", to_string(int(_direction))});
  data.insert({"empd", to_string(empAnimation.isActive())});
  p.data = map_to_string(data);

  NetworkManager::queue_packet(p);
}
