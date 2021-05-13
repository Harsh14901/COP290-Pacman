#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */

#include <iostream>

#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "Grids/WallGrid.hpp"
#include "Textures/LTexture.hpp"
#include "Network/NetworkManager.hpp"
#include "Utils/FrameGuider.hpp"

class Character {
 public:
  // The dimensions of the dot
  static const int DOT_WIDTH;
  static const int DOT_HEIGHT;
  // Maximum axis velocity of the dot
  static const int DOT_VEL = 4;

  // Initializes the variables
  Character();
  Character(string id);
  void init(SDL_Renderer* renderer);

  // Takes key presses and adjusts the dot's velocity
  void handleEvent(SDL_Event& e);

  // Moves the dot
  void move();

  // Shows the dot on the screen
  void render();

  // Place the pacman at a position.
  void place(SDL_Point p);

  int mPosX, mPosY;

  // The velocity of the dot
  int mVelX, mVelY;
  Direction _direction = Direction::LEFT;
  Direction _next = Direction::NONE;
  void change_direction(Direction d);


 protected:
  // The X and Y offsets of the dot

  void handle_collision();
  void handle_packets();
  void broadcast_coordinates();

  // The collider associated with pacman
  Collider mCollider;
  string CHARACTER_ID;
  string CHARACTER_COLLIDER_ID;
  LTexture _gDotTexture;
};