#include <SDL2/SDL_rect.h>

#include <CollisionEngine.hpp>
#include <cmath>
using namespace std;

Collider::Collider() {}
Collider::Collider(string id, SDL_Rect& rect)
    : id(id), rect(rect), t(Type::Rectangle){};
Collider::Collider(string id, Circle& circle)
    : id(id), circle(circle), t(Type::Circular){};

bool Collider::checkCollision(Collider* other) {
  if (this->t == Type::Rectangle) {
    if (other->t == Type::Rectangle) {
      return Collide_RecRec(&this->rect, &other->rect);
    } else {
      return Collide_RecCircle(&this->rect, &other->circle);
    }
  } else {
    if (other->t == Type::Circular) {
      return Collide_CircleCirlce(&this->circle, &other->circle);
    } else {
      return Collide_RecCircle(&other->rect, &this->circle);
    }
  }
}

void Collider::moveX(double x) {
  if (this->t == Type::Rectangle) {
    this->rect.x += x;
  } else {
    this->circle.first.x += x;
  }
}

void Collider::moveY(double y) {
  if (this->t == Type::Rectangle) {
    this->rect.y += y;
  } else {
    this->circle.first.y += y;
  }
}

void Collider::setX(double x) {
  if (this->t == Type::Rectangle) {
    this->rect.x = x;
  } else {
    this->circle.first.x = x;
  }
}

void Collider::setY(double y) {
  if (this->t == Type::Rectangle) {
    this->rect.y = y;
  } else {
    this->circle.first.y = y;
  }
}
void print_rect(string id, SDL_Rect& rect) {
  printf("%s : (%d,%d) , w: %d, h: %d\n", id.c_str(), rect.x, rect.y, rect.w,
         rect.h);
}

void print_point(string id, SDL_Point& p) {
  printf("%s: (%d, %d)\n", id.c_str(), p.x, p.y);
}

bool Collider::Collide_RecRec(SDL_Rect* r1, SDL_Rect* r2) {
  SDL_Rect a = *r1;
  SDL_Rect b = *r2;
  // The sides of the rectangles
  int leftA, leftB;
  int rightA, rightB;
  int topA, topB;
  int bottomA, bottomB;

  // Calculate the sides of rect A
  leftA = a.x;
  rightA = a.x + a.w;
  topA = a.y;
  bottomA = a.y + a.h;

  // Calculate the sides of rect B
  leftB = b.x;
  rightB = b.x + b.w;
  topB = b.y;
  bottomB = b.y + b.h;

  // If any of the sides from A are outside of B
  if (bottomA <= topB) {
    return false;
  }

  if (topA >= bottomB) {
    return false;
  }

  if (rightA <= leftB) {
    return false;
  }

  if (leftA >= rightB) {
    return false;
  }

  // If none of the sides from A are outside B
  return true;
}

bool Collider::Collide_CircleCirlce(Circle* c1, Circle* c2) {
  return distance(c1->first, c2->first) < c1->second + c2->second;
}

bool Collider::Collide_RecCircle(SDL_Rect* r, Circle* circle) {
  if (SDL_PointInRect(&circle->first, r)) {
    // print_point("Point in rectangle", circle->first);
    // print_rect("R", *r);
    return true;
  }

  auto p = circle->first;
  auto a = SDL_Point{r->x, r->y};
  auto b = SDL_Point{r->x + r->w, r->y};
  auto c = SDL_Point{r->x + r->w, r->y + r->h};
  auto d = SDL_Point{r->x, r->y + r->h};
  auto dis = shortest_length(a, b, p);
  dis = min(dis, shortest_length(b, c, p));
  dis = min(dis, shortest_length(c, d, p));
  dis = min(dis, shortest_length(d, a, p));

  auto ret = dis < circle->second;
  // if (ret) {
  //   print_point("center of circle", circle->first);
  //   printf("Radius of circle: %f\n", circle->second);
  //   print_rect("Rect", *r);
  //   printf("Shrotest distance : %f\n\n", dis);
  // }
  return ret;
}

double distance(SDL_Point a, SDL_Point b) {
  auto d1 = double(a.x - b.x);
  auto d2 = double(a.y - b.y);
  return sqrt(d1 * d1 + d2 * d2);
}

double shortest_length(SDL_Point A, SDL_Point B, SDL_Point E) {
  // vector AB
  SDL_Point AB;
  AB.x = B.x - A.x;
  AB.y = B.y - A.y;

  // vector BP
  SDL_Point BE;
  BE.x = E.x - B.x;
  BE.y = E.y - B.y;

  // vector AP
  SDL_Point AE;
  AE.x = E.x - A.x, AE.y = E.y - A.y;

  // Variables to store dot product
  double AB_BE, AB_AE;

  // Calculating the dot product
  AB_BE = (AB.x * BE.x + AB.y * BE.y);
  AB_AE = (AB.x * AE.x + AB.y * AE.y);

  // Minimum distance from
  // point E to the line segment
  double reqAns = 0;

  // Case 1
  if (AB_BE > 0) {
    // Finding the magnitude
    double y = E.y - B.y;
    double x = E.x - B.x;
    reqAns = sqrt(x * x + y * y);
  }

  // Case 2
  else if (AB_AE < 0) {
    double y = E.y - A.y;
    double x = E.x - A.x;
    reqAns = sqrt(x * x + y * y);
  }

  // Case 3
  else {
    // Finding the perpendicular distance
    double x1 = AB.x;
    double y1 = AB.y;
    double x2 = AE.x;
    double y2 = AE.y;
    double mod = sqrt(x1 * x1 + y1 * y1);
    reqAns = abs(x1 * y2 - y1 * x2) / mod;
  }
  return reqAns;
}

vector<Collider*> CollisionEngine::colliders = vector<Collider*>();
unordered_map<string, vector<Collider*>> CollisionEngine::collisions =
    unordered_map<string, vector<Collider*>>();

void CollisionEngine::register_collider(Collider* collider) {
  colliders.push_back(collider);
}

void CollisionEngine::deregister_collider(Collider* collider) {
  auto it = find(colliders.begin(), colliders.end(), collider);
  if (it != colliders.end()) {
    colliders.erase(it);
    collisions[collider->id].clear();
  }
}

vector<Collider*> CollisionEngine::getCollisions(string id) {
  return collisions[id];
}

void CollisionEngine::checkCollisions() {
  collisions.clear();
  int n = colliders.size();
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (colliders[i]->checkCollision(colliders[j])) {
        // cout << colliders[i]->id << " collided with " << colliders[j]->id
        //      << endl;
        collisions[colliders[i]->id].push_back(colliders[j]);
        collisions[colliders[j]->id].push_back(colliders[i]);
      }
    }
  }
  // cout << "--------" << endl;
}
