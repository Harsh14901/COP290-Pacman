#include "Enemy.hpp"

Enemy::Enemy() : Character(ENEMY_COLLIDER_ID + "_" + to_string(rand())) {}

void Enemy::handleEvent(SDL_Event& event) {}


void Enemy::init(SDL_Renderer* renderer,int enemy_type) {
  cout << ENEMY_COLLIDER_ID << endl;
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
    if(collisions[i]->id.find(COIN_COLLIDER_ID)!=-1){
      i++;
      continue;
    }
    cout << "Collision with " << collisions[i]->id << endl;
    if(collisions[i]->id.find(ENEMY_COLLIDER_ID)!=-1){
      i++;
      cout << "Enemy Collided with another enemy" << endl;
      continue;
    }
    cout << "Collision with something: " << collisions[i]->id << endl;
    switch (_direction) {
      case Direction::LEFT:
      case Direction::RIGHT:
        mPosX -= 1 * mVelX;
        break;
      case Direction::UP:
      case Direction::DOWN:
        mPosY -= 1 * mVelY;
        break;
      default:
        break;
    }
    break;
  }
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

    // Move the dot left or right
  if (!is_server) {
    handle_packets();
    return;
  }
  handle_collision();
  if (_next != Direction::NONE) {
    change_direction(_next);
  }
  mPosX += mVelX;

  // If the dot went too far to the left or right
  if ((mPosX < 0) || (mPosX + DOT_WIDTH > SCREEN_WIDTH)) {
    // Move back
    if (mPosX < 0)
      mPosX = 0;
    else
      mPosX = SCREEN_WIDTH - DOT_WIDTH;

    mVelX = 0;
  }

  // Move the dot up or down
  mPosY += mVelY;

  // If the dot went too far up or down
  if ((mPosY < 0) || (mPosY + DOT_HEIGHT > SCREEN_HEIGHT)) {
    // Move back
    if (mPosY < 0)
      mPosY = 0;
    else
      mPosY = SCREEN_HEIGHT - DOT_HEIGHT;

    mVelY = 0;
  }

  // Change rectangular collider position
  // mCollider.setX(mPosX);
  // mCollider.setY(mPosY);

  // Change circular collider position
  mCollider.setX(mPosX + PACMAN_RENDER_WIDTH / 2);
  mCollider.setY(mPosY + PACMAN_RENDER_HEIGHT / 2);
  broadcast_coordinates();

}


