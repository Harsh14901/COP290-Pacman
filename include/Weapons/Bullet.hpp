#pragma once
#include <SDL2/SDL.h>

#include <functional>
#include <iostream>

#include "Base.hpp"
#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "Textures/LTexture.hpp"
#include "Utils/AudioAsset.hpp"
#include "Utils/FrameGuider.hpp"
#include "utils.hpp"

using namespace std;

class Bullet : public Base {
 public:
  Bullet(string asset, string id, string target);
  Bullet(string asset, int velocity, string id, string target);

  void shoot(Direction dir, int x, int y);

  void move() override;
  void render() override;
  bool is_active();

  string target;

 protected:
  bool isActive = false;
  int angle = 0;

  void init_collider() override;
  void init_targets() override;
  void target_hit(string target_id, Collider* collider = nullptr) override;
};