#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include "CollisionEngine.hpp"
#include "LTexture.hpp"
#include "Constants.hpp"

#include <stdlib.h> /* srand, rand */
#include <time.h>	/* time */
#include <WallGrid.hpp>

class Pacman {
 public:
  // The dimensions of the dot
  static const int DOT_WIDTH;
  static const int DOT_HEIGHT;
  // Maximum axis velocity of the dot
  static const int DOT_VEL = 2;

  // Initializes the variables
  Pacman();
  Pacman(string id);
  void init(SDL_Renderer* renderer);

  // Takes key presses and adjusts the dot's velocity
  void handleEvent(SDL_Event& e);

  // Moves the dot
  void move();

  // Shows the dot on the screen
  void render();

  // Place the pacman at a position.
  void place(SDL_Point p);

 protected:
  // The X and Y offsets of the dot
  int mPosX, mPosY;

  // The velocity of the dot
  int mVelX, mVelY;
  Direction _direction = Direction::LEFT;

  void change_direction(Direction d);
  void handle_collision();

  // The collider associated with pacman
  Collider mCollider;
  string PACMAN_ID;
  LTexture _gDotTexture;
};

class Enemy: public Pacman{
public:
  Enemy();
  void handleEvent(SDL_Event& e);
  void move();
};