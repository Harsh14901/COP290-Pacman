#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include "CollisionEngine.hpp"
#include "LTexture.hpp"
#include "Constants.hpp"

class Pacman {
 public:
  // The dimensions of the dot
  static const int DOT_WIDTH;
  static const int DOT_HEIGHT;
  static const string PACMAN_ID;
  // Maximum axis velocity of the dot
  static const int DOT_VEL = 4;

  // Initializes the variables
  Pacman();
  void init(SDL_Renderer* renderer);

  // Takes key presses and adjusts the dot's velocity
  void handleEvent(SDL_Event& e);

  // Moves the dot
  void move();

  // Shows the dot on the screen
  void render();

  // Place the pacman at a position.
  void place(int posX, int posY);

 private:
  // The X and Y offsets of the dot
  int mPosX, mPosY;

  // The velocity of the dot
  int mVelX, mVelY;
  LTexture _gDotTexture;
  int _direction = 0;

  // The collider associated with pacman
  Collider mCollider;
};