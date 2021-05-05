#include "Characters/Character.hpp"

#include "Network/NetworkManager.hpp"

// TODO: Fix This
const int Character::DOT_WIDTH = PACMAN_RENDER_WIDTH;
const int Character::DOT_HEIGHT = PACMAN_RENDER_HEIGHT;

void Character::render() {
  // Show the dot
  SDL_Rect rect{DOT_WIDTH * int(_direction), 0, DOT_WIDTH, DOT_HEIGHT};
  _gDotTexture.render(mPosX, mPosY, &rect);
}

void Character::place(SDL_Point p) {
  mPosX = p.x;
  mPosY = p.y;
}

void Character::init(SDL_Renderer* renderer) {
  cout << CHARACTER_ID << endl;
  _gDotTexture.setRenderer(renderer);
  _gDotTexture.loadFromFile("assets/pngs/pac-classic_c-toy.png");
  _gDotTexture.set_image_dimenstions(DOT_WIDTH, DOT_HEIGHT);
  CollisionEngine::register_collider(&mCollider);
}

void Character::handleEvent(SDL_Event& e){
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

Character::Character() : Character(CHARACTER_COLLIDER_ID) {}

Character::Character(string id) {
  CHARACTER_ID = id;
  CHARACTER_COLLIDER_ID = id;
  
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
  auto circle = Circle{SDL_Point{mPosX + DOT_WIDTH / 2, mPosY + DOT_HEIGHT / 2},
                       max(DOT_WIDTH, DOT_WIDTH) / 2};
  mCollider = Collider(CHARACTER_COLLIDER_ID, circle);
}

void Character::handle_collision() {
  auto collisions = CollisionEngine::getCollisions(CHARACTER_ID);
  int i = 0;
  while (i < collisions.size()) {
    if (collisions[i]->id.find(COIN_COLLIDER_ID) != -1) {
      i++;
      continue;
    }
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

void Character::handle_packets() {
  vector<Packet> packets;
  NetworkManager::get_packets(CHARACTER_ID, packets);

  for (auto& p : packets) {
    mPosX = p.posX;
    mPosY = p.posY;
    mVelX = p.velX;
    mVelY = p.velY;
    _direction = Direction(stoi(p.data));
  }
}

void Character::broadcast_coordinates() {
  Packet p;
  p.id = CHARACTER_ID;
  p.posX = mPosX;
  p.posY = mPosY;
  p.velX = mVelX;
  p.velY = mVelY;
  p.data = to_string(int(_direction));
  NetworkManager::queue_packet(p);
}

void Character::change_direction(Direction d) {
  if (WallGrid::can_move(mPosX + DOT_WIDTH / 2, mPosY + DOT_HEIGHT / 2, d)) {
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
    _next = Direction::NONE;
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