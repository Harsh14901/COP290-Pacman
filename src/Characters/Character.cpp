#include "Characters/Character.hpp"

#include "Utils/AssetManager.hpp"
#include "utils.hpp"

Character::Character(string id)
    : Character(id, AssetManager::get_asset(ThemeAssets::PACMAN_SPRITE)) {}

Character::Character(string id, string asset)
    : Base(PACMAN_RENDER_WIDTH, PACMAN_RENDER_HEIGHT, PLAYER_VEL, id, asset) {
  COLLIDER_ID = id;
  EMP_VEL = PLAYER_VEL / 2;
  FROZEN_VEL = 0;

  empAnimation.set_duration(200);
  freezeAnimation.set_duration(100);
}
void Character::init(SDL_Renderer* renderer) { Base::init(renderer); }
void Character::init(SDL_Renderer* renderer,
                     pair<BulletType, BulletType> equipped_bullets) {
  this->equipped_bullets = equipped_bullets;
  weaponSet.primary_weapon.init(equipped_bullets.first, this, 1);
  weaponSet.secondary_weapon.init(equipped_bullets.second, this, 1);
  Character::init(renderer);
}

void Character::init_targets() {
  auto b1 = equipped_bullets.first;
  auto b2 = equipped_bullets.second;

  if (b1 != BulletType::FREEZE && b2 != BulletType::FREEZE) {
    add_target(IDS::FREEZEBULLET_ID);
    printf("%s - %s\n", ID.c_str(), IDS::FREEZEBULLET_ID.c_str());
  }
  if (b1 != BulletType::GRENADE && b2 != BulletType::GRENADE) {
    add_target(IDS::GRENADE_ID);
    printf("%s - %s\n", ID.c_str(), IDS::GRENADE_ID.c_str());
  }
  if (b1 != BulletType::EMP && b2 != BulletType::EMP) {
    add_target(IDS::EMP_ID);
    printf("%s - %s\n", ID.c_str(), IDS::EMP_ID.c_str());
  }
  Base::init_targets();
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
  weaponSet.handleEvent(e);
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
  if (target_id == IDS::FREEZEBULLET_ID) {
    freeze();
  }
  if (target_id == IDS::EMP_ID) {
    emp();
  }
}

void Character::process_packet(Packet& p) {
  mPosX = p.posX;
  mPosY = p.posY;
  mVelX = p.velX;
  mVelY = p.velY;
  auto data = convert_string_to_map(p.data);
  _direction = Direction(stoi(data["direction"]));
  if (data["empd"] == "1") {
    emp(false);
  }
  if (data["frozen"] == "1") {
    freeze(false);
  }
}

void Character::handle_packets() {
  vector<Packet> packets;
  NetworkManager::get_packets(ID, packets);

  for (auto& p : packets) {
    process_packet(p);
  }
}

Packet Character::make_packet(unordered_map<string, string>& data) {
  Packet p;
  p.id = ID;
  p.posX = mPosX;
  p.posY = mPosY;
  p.velX = mVelX;
  p.velY = mVelY;
  data.insert({"direction", to_string(int(_direction))});
  data.insert({"empd", to_string(is_empd)});
  data.insert({"frozen", to_string(freezeAnimation.isActive())});
  return p;
}

void Character::broadcast_coordinates() {
  unordered_map<string, string> data;
  auto p = make_packet(data);
  p.data = map_to_string(data);
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
  check_emp();
  check_frozen();
  // printf("%s - %d, %d\n", ID.c_str(), mVelX, mVelY);

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
string Character::get_weapon_text() { return weaponSet.get_text(); }

void Character::emp(bool broadcast) {
  // cout << "Getting EMPD" << endl;
  empAnimation.start();
  if (broadcast) {
    broadcast_coordinates();
  }
}

void Character::freeze(bool broadcast) {
  // cout << "Time to Freeze" << endl;
  freezeAnimation.start();
  if (broadcast) {
    broadcast_coordinates();
  }
}

void Character::check_emp() {
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

void Character::check_frozen() {
  auto swap_vels = [&]() {
    auto temp = FROZEN_VEL;
    FROZEN_VEL = MAX_VEL;
    MAX_VEL = temp;
  };
  if (freezeAnimation.isActive() && !is_frozen) {
    swap_vels();
    is_frozen = true;
  }
  if (!freezeAnimation.isActive() && is_frozen) {
    swap_vels();
    is_frozen = false;
  }
}
