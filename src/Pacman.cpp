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
    mVelY = 0;
    mVelX = 0;
    // Adjust the velocity
    switch (e.key.keysym.sym) {
      case SDLK_UP:
        mVelY -= DOT_VEL;
        _direction = 2;
        break;
      case SDLK_DOWN:
        mVelY += DOT_VEL;
        _direction = 3;
        break;
      case SDLK_LEFT:
        mVelX -= DOT_VEL;
        _direction = 0;
        break;
      case SDLK_RIGHT:
        mVelX += DOT_VEL;
        _direction = 1;
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
  SDL_Rect rect{DOT_WIDTH * _direction, 0, DOT_WIDTH, DOT_HEIGHT};
  _gDotTexture.render(mPosX, mPosY, &rect);
}

void Pacman::move() {
  // Move the dot left or right
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
  auto collisions = CollisionEngine::getCollisions(PACMAN_ID);

  if (!collisions.empty()) {
    switch (_direction) {
      case 0:
        mPosX -= 2 * mVelX;
        mVelX *= 0;
        _direction = 1;
        break;
      case 1:
        mPosX -= 2 * mVelX;
        mVelX *= 0;
        _direction = 0;
        break;
      case 2:
        mPosY -= 2 * mVelY;
        mVelY *= 0;
        _direction = 3;
        break;
      case 3:
        mPosY -= 2 * mVelY;
        mVelY *= 0;
        _direction = 2;
        break;
      default:
        break;
    }
  }

  // for (auto& c : collisions) {
  //   cout << "Pacman has collided with " << c->id << endl;
  // }
  // cout << "------------" << endl;

  // Change rectangular collider position
  // mCollider.setX(mPosX);
  // mCollider.setY(mPosY);

  // Change circular collider position
  mCollider.setX(mPosX + PACMAN_RENDER_WIDTH / 2);
  mCollider.setY(mPosY + PACMAN_RENDER_HEIGHT / 2);
}

Enemy::Enemy()
    : Pacman(ENEMY_COLLIDER_ID + "_" + to_string(rand())), counter(0) {}

void Enemy::handleEvent(SDL_Event& event) {}

void Enemy::move() {
  if (counter == 0) {
    _direction = rand() % 4;
  }
  mVelY = 0;
  mVelX = 0;
  switch (_direction) {
    case 0:
      mVelX = -DOT_VEL;
      break;
    case 1:
      mVelX = DOT_VEL;
      break;
    case 2:
      mVelY = -DOT_VEL;
      break;
    case 3:
      mVelY = DOT_VEL;
      break;
    default:
      break;
  }
  counter = (counter + 1) % RETAIN_DIRECTION_FOR_FRAMES;
  Pacman::move();
}