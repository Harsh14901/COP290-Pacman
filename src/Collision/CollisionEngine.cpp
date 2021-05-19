
#include <Collision/CollisionEngine.hpp>
using namespace std;

vector<Collider*> CollisionEngine::colliders = vector<Collider*>();
unordered_map<string, vector<Collider*>> CollisionEngine::collisions =
    unordered_map<string, vector<Collider*>>();

void CollisionEngine::register_collider(Collider* collider) {
  colliders.push_back(collider);
}

void CollisionEngine::deregister_collider(Collider* collider) {
  if (collider == nullptr) {
    return;
  }
  auto it = find(colliders.begin(), colliders.end(), collider);
  if (it != colliders.end()) {
    colliders.erase(it);
    if (!collisions[collider->id].empty()) {
      collisions[collider->id].clear();
    }
  }
}

vector<Collider*> CollisionEngine::getCollisions(string id) {
  return collisions[id];
}

void CollisionEngine::clear_all() {
  collisions.clear();
  colliders.clear();
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
