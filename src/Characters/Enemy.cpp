#include "Characters/Enemy.hpp"

vector<int> Enemy::ids;
int Enemy::active_id;

Enemy::Enemy() : Character(ENEMY_COLLIDER_ID + "_" + to_string(rand())) {
  if (ids.empty()) {
    id = 0;
    active_id = 0;
  } else {
    auto prev_id = ids.back();
    id = prev_id + 1;
  }
  ids.push_back(id);
}

void Enemy::switch_active_id() {
  if (active_id == ids.back()) {
    active_id = ids.front();
  } else {
    active_id++;
  }
}
void Enemy::handleEvent(SDL_Event& e) {
  if (id != active_id) {
    return;
  }
  Character::handleEvent(e);
}

void Enemy::init(SDL_Renderer* renderer, int enemy_type) {
  cout << CHARACTER_COLLIDER_ID << endl;

  _gDotTexture.setRenderer(renderer);
  _gDotTexture.loadFromFile("assets/pngs/pac-classic_c-toy.png");
  _gDotTexture.set_image_dimenstions(DOT_WIDTH, DOT_HEIGHT);

  CollisionEngine::register_collider(&mCollider);
  type = enemy_type;
}

void Enemy::handle_collision() {
  auto collisions = CollisionEngine::getCollisions(CHARACTER_COLLIDER_ID);
  int i = 0;

  while (i < collisions.size()) {
    if (collisions[i]->id.find(COIN_COLLIDER_ID) != -1 ||
        collisions[i]->id.find(CHERRY_COLLIDER_ID) != -1) {
      i++;
      continue;
    }
    // cout << "Collision with " << collisions[i]->id << endl;
    if (collisions[i]->id.find(ENEMY_COLLIDER_ID) != -1) {
      i++;
      // cout << "Enemy Collided with another enemy" << endl;
      continue;
    }
    // cout << "Collision with something: " << collisions[i]->id << endl;
    Character::handle_collision();
    break;
  }
}

void Enemy::render() {
  // Show the dot
  int typeValue = state == EnemyState::WEAK ? type : type;
  SDL_Rect rect{138 * (2 + int(_direction) % 2), 171 * typeValue, 138, 171};
  _gDotTexture.render(mPosX, mPosY, &rect, 90 * (int(_direction) / 2));
}

void Enemy::randomize_direction() {
  unordered_set<Direction> available_directions;
  for (int i = 0; i < 4; i++) {
    auto d = Direction(i);

    if (WallGrid::can_move(mPosX + PACMAN_RENDER_WIDTH / 2,
                           mPosY + PACMAN_RENDER_HEIGHT / 2, d)) {
      // printf("Direction %d is available\n", i);
      available_directions.insert(d);
    }
  }

  if (_direction == Direction::LEFT || _direction == Direction::RIGHT) {
    available_directions.erase(Direction::LEFT);
    available_directions.erase(Direction::RIGHT);
  } else {
    available_directions.erase(Direction::UP);
    available_directions.erase(Direction::DOWN);
  }
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
    // cout << "No available direction for " << PACMAN_ID << endl;
    // printf("Velocities: %d, %d\n", mVelX, mVelY);
    // change_direction(Direction(rand() % 4));
    change_direction(_direction);
  }
}

void Enemy::move() {
  if (is_server) {
    handle_packets();
    handle_collision();
    if (!is_two_player) {
      randomize_direction();
    }
    Character::move();
    return;
  }
  handle_collision();
  if (active_id != id) {
    randomize_direction();
  }
  Character::move();
}

void Enemy::setState(EnemyState st) { state = st; }
