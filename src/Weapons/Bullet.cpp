#include "Weapons/Bullet.hpp"

#include "Network/NetworkManager.hpp"

Bullet::Bullet(string asset, string id) : Bullet(asset, BULLET_VEL, id) {}
Bullet::Bullet(string asset, int velocity, string id)
    : Base(32, 32, velocity, id, asset) {}

void Bullet::init_collider() {
  auto rect = SDL_Rect{-1, -1, 5, 5};
  mCollider = Collider(ID, rect);
}

void Bullet::shoot(Direction dir, int x, int y) {
  mPosX = x;
  mPosY = y;
  if (dir == Direction::DOWN) {
    mVelX = 0;
    mVelY = MAX_VEL;
    angle = 180;
  } else if (dir == Direction::UP) {
    mVelX = 0;
    mVelY = -MAX_VEL;
    angle = 0;
  } else if (dir == Direction::LEFT) {
    mVelX = -MAX_VEL;
    mVelY = 0;
    angle = -90;
  } else {
    mVelY = 0;
    mVelX = MAX_VEL;
    angle = 90;
  }
  _direction = dir;
  isActive = true;
}

void Bullet::handle_collision() {
  auto collisions = CollisionEngine::getCollisions(ID);

  for (auto& item : collisions) {
    if (item->id.find(IDS::WALL_COLLIDER_ID) != -1) {
      auto temp = extractIntegerWords(item->id);
      if (temp.size() == 2) {
        isActive = false;
      }
    }
  }
}

bool Bullet::is_active() {
  if (mPosX > GAMEAREA_WIDTH || mPosY > GAMEAREA_HEIGHT || mPosX < 0 ||
      mPosY < 0) {
    isActive = false;
  }
  return isActive;
}

void Bullet::move() {
  handle_collision();

  if (!is_active()) {
    return;
  }

  mPosX += mVelX * FrameGuider::getFrameDeltaRounded();
  mPosY += mVelY * FrameGuider::getFrameDeltaRounded();

  mCollider.setX(mPosX);
  mCollider.setY(mPosY);
}

void Bullet::render() {
  if (!is_active()) return;

  _gDotTexture.render(mPosX, mPosY, NULL, angle);
}
