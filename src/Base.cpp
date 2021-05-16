#include "Base.hpp"

Base::Base(int width, int height, int vel, string id, string asset)
    : WIDTH(width), HEIGHT(height), MAX_VEL(vel), ID(id), ASSET(asset) {}

void Base::init(SDL_Renderer* renderer) {
  cout << ID << endl;
  _gDotTexture.setRenderer(renderer);
  _gDotTexture.loadFromFile(ASSET);
  _gDotTexture.set_image_dimenstions(WIDTH, HEIGHT);
  init_collider();
  init_targets();
  CollisionEngine::register_collider(&mCollider);
}
void Base::handleEvent(SDL_Event& e) {}

void Base::move() {}

void Base::render() {}

void Base::place(SDL_Point p) {
  mPosX = p.x;
  mPosY = p.y;
}

void Base::change_direction(Direction d) {}

void Base::init_collider() {
  auto rect = SDL_Rect{-1, -1, WIDTH, HEIGHT};
  mCollider = Collider(ID, rect);
}

void Base::init_targets() { add_target(IDS::WALL_COLLIDER_ID); }

void Base::add_target(string target_id) {
  if (find(collider_targets.begin(), collider_targets.end(), target_id) ==
      collider_targets.end()) {
    collider_targets.push_back(target_id);
  }
}

void Base::handle_collision() {
  auto collisions = CollisionEngine::getCollisions(ID);

  for (auto& collider : collisions) {
    for (auto& target : collider_targets) {
      if (collider->id.find(target) != string::npos) {
        target_hit(target, collider);
      }
    }
  }
}

void Base::target_hit(string target_id, Collider* collider) {}
void Base::handle_packets() {}
void Base::broadcast_coordinates() {}

Base::~Base() {
  CollisionEngine::deregister_collider(&mCollider);
  cout << "Exiting base" << endl;
}