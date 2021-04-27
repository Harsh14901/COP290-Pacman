#pragma once

#include <bits/stdc++.h>

#include <cmath>

#include "Collision/Collider.hpp"

using namespace std;

class CollisionEngine {
 private:
  static vector<Collider*> colliders;
  static unordered_map<string, vector<Collider*>> collisions;

 public:
  // register a collider with collision engine
  static void register_collider(Collider* collider);
  // deregister a collider with collision engine
  static void deregister_collider(Collider* collider);

  // Returns the collider pointers with which the collider with argument id has
  // collided
  static vector<Collider*> getCollisions(string id);

  // Checks for collision among registered colliders
  static void checkCollisions();
};