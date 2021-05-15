#include "Base.hpp"

Base::Base(int width, int height, int vel, string id, string asset)
    : WIDTH(width), HEIGHT(height), MAX_VEL(vel), ID(id), ASSET(asset) {}

void Base::init(SDL_Renderer* renderer) {
  cout << ID << endl;
  _gDotTexture.setRenderer(renderer);
  _gDotTexture.loadFromFile(ASSET);
  _gDotTexture.set_image_dimenstions(WIDTH, HEIGHT);
  init_collider();
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

void Base::handle_collision() {}
void Base::handle_packets() {}
void Base::broadcast_coordinates() {}

Base::~Base() {}