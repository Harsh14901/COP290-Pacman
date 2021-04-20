#pragma once

#include <bits/stdc++.h>
using namespace std;

typedef pair<SDL_Point, double> Circle;

class Collider {
 public:
  enum class Type { Rectangle, Circular };
  string id;

  Collider();
  Collider(string id, SDL_Rect& rect);
  Collider(string id, Circle& circle);
  bool checkCollision(Collider* other);
  void moveX(double x);
  void moveY(double y);
  void setX(double x);
  void setY(double y);

  static bool Collide_RecCircle(SDL_Rect* r, Circle* c);
  static bool Collide_RecRec(SDL_Rect* r1, SDL_Rect* r2);
  static bool Collide_CircleCirlce(Circle* c1, Circle* c2);

 private:
  SDL_Rect rect;
  Circle circle;
  Type t;
};

double distance(SDL_Point a, SDL_Point b);
double shortest_length(SDL_Point a, SDL_Point b, SDL_Point e);

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