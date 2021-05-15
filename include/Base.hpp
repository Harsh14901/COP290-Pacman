#pragma once
#include <SDL2/SDL.h>

#include <string>

#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "Textures/LTexture.hpp"

class Base {
 public:
  // The dimensions of the dot
  int WIDTH;
  int HEIGHT;
  // Maximum axis velocity of the dot
  int MAX_VEL;
  string ID;
  string ASSET;

  Direction _direction = Direction::LEFT;
  int mPosX = 0, mPosY = 0;
  int mVelX = 0, mVelY = 0;

  Base(int width, int height, int vel, string id, string asset);
  virtual void init(SDL_Renderer* renderer);

  // Takes key presses and adjusts the dot's velocity
  virtual void handleEvent(SDL_Event& e);

  // Moves the dot
  virtual void move();

  // Shows the dot on the screen
  virtual void render();

  // Place the pacman at a position.
  virtual void place(SDL_Point p);

  virtual void change_direction(Direction d);

  virtual ~Base();

 protected:
  // The collider associated with pacman
  Collider mCollider;
  LTexture _gDotTexture;

  virtual void init_collider();
  virtual void handle_collision();
  virtual void handle_packets();
  virtual void broadcast_coordinates();
};