#include "Pacman.hpp"

const int Pacman::DOT_WIDTH = PACMAN_DOT_WIDTH;
const int Pacman::DOT_HEIGHT = PACMAN_DOT_HEIGHT;

Pacman::Pacman() : Pacman(PACMAN_COLLIDER_ID) {}

Pacman::Pacman(string id) {
  PACMAN_ID = id;
  // Initialize the offsets
  mPosX = 0;
  mPosY = 0;

  // Initialize the velocity
  mVelX = 0;
  mVelY = 0;

  // Initialize collider

  // Rectangular collider
  // auto rect = SDL_Rect{mPosX, mPosY, PACMAN_RENDER_WIDTH,
  // PACMAN_RENDER_HEIGHT}; mCollider = Collider(PACMAN_ID, rect);

  // Circular collider
  auto circle = Circle{SDL_Point{mPosX + PACMAN_RENDER_WIDTH / 2,
                                 mPosY + PACMAN_RENDER_HEIGHT / 2},
                       max(PACMAN_RENDER_WIDTH, PACMAN_RENDER_HEIGHT) / 2};
  mCollider = Collider(PACMAN_ID, circle);
}

void Pacman::init(SDL_Renderer* renderer) {
  cout << PACMAN_ID << endl;
  _gDotTexture.setRenderer(renderer);
  _gDotTexture.loadFromFile("assets/bitmaps/pacman.bmp");
  _gDotTexture.set_image_dimenstions(PACMAN_RENDER_WIDTH, PACMAN_RENDER_HEIGHT);
  CollisionEngine::register_collider(&mCollider);
}

void Pacman::handleEvent(SDL_Event& e) {
  // If a key was pressed
  if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
    // Adjust the velocity
    handle_collision();
    switch (e.key.keysym.sym) {
      case SDLK_UP:
        change_direction(Direction::UP);
        break;
      case SDLK_DOWN:
        change_direction(Direction::DOWN);
        break;
      case SDLK_LEFT:
        change_direction(Direction::LEFT);
        break;
      case SDLK_RIGHT:
        change_direction(Direction::RIGHT);
        break;
    }
  }
}

void Pacman::handle_collision() {
  auto collisions = CollisionEngine::getCollisions(PACMAN_ID);

  if (!collisions.empty()) {
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
  }
  // for (auto& c : collisions) {
  //   cout << "Pacman has collided with " << c->id << endl;
  // }
  // cout << "------------" << endl;
}

void Pacman::change_direction(Direction d) {
  if (WallGrid::can_move(mPosX + PACMAN_RENDER_WIDTH / 2,
                         mPosY + PACMAN_RENDER_HEIGHT / 2, d)) {
    mVelX = 0;
    mVelY = 0;
    switch (d) {
      case Direction::LEFT:
        mVelX = -DOT_VEL;
        _direction = Direction::LEFT;
        break;
      case Direction::RIGHT:
        mVelX = DOT_VEL;
        _direction = Direction::RIGHT;
        break;
      case Direction::UP:
        mVelY = -DOT_VEL;
        _direction = Direction::UP;
        break;
      case Direction::DOWN:
        mVelY = DOT_VEL;
        _direction = Direction::DOWN;
        break;
      default:
        break;
    }
  }
}

void Pacman::place(SDL_Point p) {
  mPosX = p.x;
  mPosY = p.y;
}

void Pacman::render() {
  // Show the dot
  SDL_Rect rect{DOT_WIDTH * int(_direction), 0, DOT_WIDTH, DOT_HEIGHT};
  _gDotTexture.render(mPosX, mPosY, &rect);
}

void Pacman::move() {
  // Move the dot left or right
  handle_collision();

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
}

Enemy::Enemy() : Pacman(ENEMY_COLLIDER_ID + "_" + to_string(rand())) {}

void Enemy::handleEvent(SDL_Event& event) {}

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
  Pacman::move();
}