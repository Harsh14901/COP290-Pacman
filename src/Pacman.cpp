#include "Pacman.hpp"

const string Pacman::PACMAN_ID = "pacman";
const int Pacman::DOT_WIDTH = 34;
const int Pacman::DOT_HEIGHT = 40;

Pacman::Pacman() {
  // Initialize the offsets
  mPosX = 0;
  mPosY = 0;

  // Initialize the velocity
  mVelX = 0;
  mVelY = 0;

  // Initialize collider

  // Rectangular collider
  // auto rect = SDL_Rect{mPosX, mPosY, DOT_WIDTH, DOT_HEIGHT};
  // mCollider = Collider(PACMAN_ID, rect);

  // Circular collider
  auto circle = Circle{SDL_Point{mPosX + DOT_WIDTH/2, mPosY + DOT_HEIGHT/2}, max(DOT_WIDTH, DOT_HEIGHT)/2};
  mCollider = Collider(PACMAN_ID, circle);
}

void Pacman::init(SDL_Renderer* renderer) {
  _gDotTexture.setRenderer(renderer);
  _gDotTexture.loadFromFile("assets/bitmaps/pacman.bmp");
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
    cout << "Oh Yeag" << endl;
    // Move back
    if (mPosY < 0)
      mPosY = 0;
    else
      mPosY = SCREEN_HEIGHT - DOT_HEIGHT;

    mVelY = 0;
  }

  auto collisions = CollisionEngine::getCollisions(PACMAN_ID);
  for(auto& c: collisions){
    cout<<"Pacman has collided with "<<c->id<<endl;
  }
  cout<<"------------"<<endl;

  // Change rectangular collider position
  // mCollider.setX(mPosX);
  // mCollider.setY(mPosY);

  // Change circular collider position
  mCollider.setX(mPosX + DOT_WIDTH/2);
  mCollider.setY(mPosY + DOT_HEIGHT/2);
}
