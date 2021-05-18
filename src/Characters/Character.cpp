#include "Characters/Character.hpp"
#include "Utils/AssetManager.hpp"
#include "utils.hpp"

extern AssetManager assetManager;

Character::Character(string id)
    : Character(id, assetManager.get_asset(ThemeAssets::PACMAN_SPRITE)) {}

Character::Character(string id, string asset)
    : Base(PACMAN_RENDER_WIDTH, PACMAN_RENDER_HEIGHT, PLAYER_VEL, id, asset) {
  COLLIDER_ID = id;
}

void Character::init_collider() {
  // Initialize collider

  // Rectangular collider
  // auto rect = SDL_Rect{mPosX, mPosY, PACMAN_RENDER_WIDTH,
  // PACMAN_RENDER_HEIGHT}; mCollider = Collider(PACMAN_ID, rect);

  // Circular collider
  auto circle = Circle{SDL_Point{mPosX + WIDTH / 2, mPosY + HEIGHT / 2},
                       max(WIDTH, WIDTH) / 2};
  mCollider = Collider(COLLIDER_ID, circle);
}

void Character::render() {
  // Show the dot
  SDL_Rect rect{WIDTH * int(_direction), 0, WIDTH, HEIGHT};
  _gDotTexture.render(mPosX, mPosY, &rect);
}

void Character::handleEvent(SDL_Event& e) {
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

void Character::target_hit(string target_id, Collider* collider) {
  if (target_id == IDS::WALL_COLLIDER_ID) {
    switch (_direction) {
      case Direction::LEFT:
      case Direction::RIGHT:
        mPosX -= 1 * mVelX * FrameGuider::getFrameDeltaRounded();
        break;
      case Direction::UP:
      case Direction::DOWN:
        mPosY -= 1 * mVelY * FrameGuider::getFrameDeltaRounded();
        break;
      default:
        break;
    }
  }
}

void Character::handle_packets() {
  vector<Packet> packets;
  NetworkManager::get_packets(ID, packets);

  for (auto& p : packets) {
    mPosX = p.posX;
    mPosY = p.posY;
    mVelX = p.velX;
    mVelY = p.velY;
    _direction = Direction(stoi(p.data));
  }
}

void Character::broadcast_coordinates() {
  // cout << "ID is " << ID << endl;
  Packet p;
  p.id = ID;
  p.posX = mPosX;
  p.posY = mPosY;
  p.velX = mVelX;
  p.velY = mVelY;
  p.data = to_string(int(_direction));
  NetworkManager::queue_packet(p);
}

void Character::change_direction(Direction d) {
  if (WallGrid::getInstance()->can_move(mPosX + WIDTH / 2, mPosY + HEIGHT / 2,
                                        d)) {
    mVelX = 0;
    mVelY = 0;
    switch (d) {
      case Direction::LEFT:
        mVelX = -MAX_VEL;
        _direction = Direction::LEFT;
        break;
      case Direction::RIGHT:
        mVelX = MAX_VEL;
        _direction = Direction::RIGHT;
        break;
      case Direction::UP:
        mVelY = -MAX_VEL;
        _direction = Direction::UP;
        break;
      case Direction::DOWN:
        mVelY = MAX_VEL;
        _direction = Direction::DOWN;
        break;
      default:
        break;
    }
    _next = Direction::NONE;

    // TODO: Remove this hack
    // if (ID.find(IDS::PACMAN_COLLIDER_ID) == -1) broadcast_coordinates();
    broadcast_coordinates();
  } else {
    _next = d;
  }
}

void Character::move() {
  // Move the dot left or right
  if (_next != Direction::NONE) {
    change_direction(_next);
  }
  mPosX += mVelX * FrameGuider::getFrameDeltaRounded();

  // If the dot went too far to the left or right
  if ((mPosX < 0) || (mPosX + WIDTH > GAMEAREA_WIDTH)) {
    // Move back
    if (mPosX < 0)
      mPosX = 0;
    else
      mPosX = GAMEAREA_WIDTH - WIDTH;

    mVelX = 0;
  }

  // Move the dot up or down
  mPosY += mVelY * FrameGuider::getFrameDeltaRounded();

  // If the dot went too far up or down
  if ((mPosY < 0) || (mPosY + HEIGHT > GAMEAREA_HEIGHT)) {
    // Move back
    if (mPosY < 0)
      mPosY = 0;
    else
      mPosY = GAMEAREA_HEIGHT - HEIGHT;

    mVelY = 0;
  }

  // Change rectangular collider position
  // mCollider.setX(mPosX);
  // mCollider.setY(mPosY);

  // Change circular collider position
  mCollider.setX(mPosX + WIDTH / 2);
  mCollider.setY(mPosY + HEIGHT / 2);
}