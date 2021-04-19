#include "Pacman.hpp"

Pacman::Pacman() {
  // Initialize the offsets
  mPosX = 0;
  mPosY = 0;

  // Initialize the velocity
  mVelX = 0;
  mVelY = 0;
}

void Pacman::init(SDL_Renderer* renderer) {
  _gDotTexture.setRenderer(renderer);
  _gDotTexture.loadFromFile("assets/bitmaps/pacman.bmp");
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
}
