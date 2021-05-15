#pragma once
#include <SDL2/SDL.h>

#include <functional>
#include <iostream>

#include "Base.hpp"
#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "Textures/LTexture.hpp"
#include "Utils/FrameGuider.hpp"
#include "utils.hpp"

using namespace std;

class BulletWeapon : public Base {
 public:
  BulletWeapon(string asset, string id);
  BulletWeapon(string asset, int velocity, string id);

  void shoot(Direction dir, int x, int y);

  void move() override;
  void render() override;

 protected:
  bool isLaunched = false;
  int angle = 0;

  void init_collider() override;
  void handle_collision() override;
  void handle_packets() override;
  void broadcast_coordinates() override;
};