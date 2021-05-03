#include "Enemy.hpp"

Enemy::Enemy() : Character(ENEMY_COLLIDER_ID + "_" + to_string(rand())) {}

void Enemy::handleEvent(SDL_Event& event) {}


void Enemy::init(SDL_Renderer* renderer,int enemy_type) {
  cout << CHARACTER_ID << endl;
  _gDotTexture.setRenderer(renderer);
  _gDotTexture.loadFromFile("assets/pngs/pac-classic_c-toy.png");
  _gDotTexture.set_image_dimenstions(DOT_WIDTH, DOT_HEIGHT);
  CollisionEngine::register_collider(&mCollider);
  type = enemy_type;
}

void Enemy::render() {
  // Show the dot
  SDL_Rect rect{138*(2+int(_direction)%2) , 171 * type, 138, 171};
  _gDotTexture.render(mPosX, mPosY, &rect,90 * (int(_direction)/2));
}

void Enemy::move() {
  handle_collision();
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
  // cout << "-----------------" << endl;
  Character::move();
}