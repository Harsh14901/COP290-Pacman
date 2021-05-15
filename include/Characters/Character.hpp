#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */

#include <iostream>

#include "Base.hpp"
#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "Grids/WallGrid.hpp"
#include "Network/NetworkManager.hpp"
#include "Textures/LTexture.hpp"
#include "Utils/FrameGuider.hpp"

class Character : public Base {
 public:
  // Initializes the variables
  Character(string id);
  Character(string id, string asset);

  // Takes key presses and adjusts the dot's velocity
  void handleEvent(SDL_Event& e) override;

  // Moves the dot
  void move() override;

  // Shows the dot on the screen
  void render() override;

  void change_direction(Direction d) override;

 protected:
  // The X and Y offsets of the dot
  void init_collider() override;
  void handle_collision() override;
  void handle_packets() override;
  void broadcast_coordinates() override;

  // The collider associated with pacman
  Direction _next = Direction::NONE;
  string COLLIDER_ID;
};