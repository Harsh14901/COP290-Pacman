#include "Weapons/BulletWeapon.hpp"

#include "Network/NetworkManager.hpp"

BulletWeapon::BulletWeapon(string asset, string id)
    : BulletWeapon(asset, BULLET_VEL, id) {}
BulletWeapon::BulletWeapon(string asset, int velocity, string id)
    : Base(32, 32, velocity, id, asset) {}

void BulletWeapon::init_collider() {
  auto rect = SDL_Rect{-1, -1, 5, 5};
  mCollider = Collider(ID, rect);
}

void BulletWeapon::broadcast_coordinates() {
  if (!isLaunched) return;
  Packet p;

  p.id = ID;
  p.posX = mPosX;
  p.posY = mPosY;
  p.velX = mVelX;
  p.velY = mVelY;
  p.data = to_string(int(angle));
  NetworkManager::queue_packet(p);
}

void BulletWeapon::handle_packets() {
  vector<Packet> packets;
  NetworkManager::get_packets(ID, packets);

  for (auto& p : packets) {
    //   cout << "Server has got what it needs" << endl;
    isLaunched = true;
    mPosX = p.posX;
    mPosY = p.posY;
    mVelX = p.velX;
    mVelY = p.velY;
    angle = stoi(p.data);
  }
}

void BulletWeapon::shoot(Direction dir, int x, int y) {
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
  cout << "Launching bullet: " << this->mPosX << " , " << this->mPosY << endl;
  isLaunched = true;
}

void BulletWeapon::handle_collision() {
  auto collisions = CollisionEngine::getCollisions(ID);

  for (auto& item : collisions) {
    if (item->id.find(IDS::WALL_COLLIDER_ID) != -1) {
      auto temp = extractIntegerWords(item->id);
      if (temp.size() == 2) {
        isLaunched = false;
      }
    }
  }
}

void BulletWeapon::move() {
  if (!isLaunched) {
    cout << "Not Launched" << endl;
    return;
  }

  mPosX += mVelX * FrameGuider::getFrameDeltaRounded();
  mPosY += mVelY * FrameGuider::getFrameDeltaRounded();

  mCollider.setX(mPosX);
  mCollider.setY(mPosY);

  handle_collision();

  // handle_packets();
  // broadcast_coordinates();
}

void BulletWeapon::render() {
  if (mPosX > GAMEAREA_WIDTH || mPosY > GAMEAREA_HEIGHT || mPosX < 0 ||
      mPosY < 0) {
    isLaunched = false;
  }
  if (!isLaunched) return;
  // cout << "Rendering bullet: " << this->mPosX << " , " << this->mPosY <<
  // endl;

  _gDotTexture.render(mPosX, mPosY, NULL, angle);
}