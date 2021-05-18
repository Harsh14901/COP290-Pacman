#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "Characters/Character.hpp"
#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "Grids/ObjectGrid.hpp"
#include "Grids/WallGrid.hpp"
#include "Textures/LTexture.hpp"
#include "Utils/Animator.hpp"

class VentGrid : public ObjectGrid {
 public:
  VentGrid();

  static VentGrid* getInstance();
  void generate() override;
  void render();
  bool handleOpening(int i, int j);
  void teleportRandom();
  pair<int, int> getTeleportLocation();
  bool canTeleport();

 protected:
 private:
  static unique_ptr<VentGrid> _instance;
  vector<pair<int, int>> vents;
  Animator openingAnimator = Animator(60);
  bool isOpen = false;
  SDL_Rect getRect(int i, int j);
  pair<int, int> currentVent;
  pair<int, int> teleportLocation;
};