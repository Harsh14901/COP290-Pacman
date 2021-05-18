#include "Grids/PotionGrid.hpp"

#include "Utils/AssetManager.hpp"
using namespace std;
extern bool is_server;

const int BOOST_INTERVAL = 500;
const int BOOST_ACTIVE = 200;
const int INVISIBLE_INTERVAL = 800;
const int INVISIBLE_ACTIVE = 300;

PotionGrid::PotionGrid(string id, string asset, int interval, int active_time)
    : ObjectGrid(id, asset, 1.2) {
  startDelay.set_duration(interval);
  potionAnimator.set_duration(active_time);
  startDelay.start();
}

void PotionGrid::generate() {
  if (!is_server) {
    handle_packets();
    return;
  }
  if (startDelay.isActive() || potionAnimator.isActive()) {
    return;
  }
  if (active_objects > 0) {
    for (int i = 0; i < GRID_ROW; i++) {
      for (int j = 0; j < GRID_COL; j++) {
        if (!is_set(i, j)) {
          unset_object(i, j);
          broadcast(i, j);
        }
      }
    }
    startDelay.start();
    return;
  }
  auto empty_loc = WallGrid::getInstance()->get_empty_indices();
  set_object(empty_loc.x, empty_loc.y);
  broadcast(empty_loc.x, empty_loc.y);
  potionAnimator.start();
}

unique_ptr<BoostGrid> BoostGrid::_instance;
BoostGrid* BoostGrid::getInstance() {
  if (_instance == nullptr) {
    _instance = make_unique<BoostGrid>();
  }
  return _instance.get();
}

BoostGrid::BoostGrid()
    : PotionGrid(IDS::BOOST_ID, AssetManager::get_asset(BOOST_PNG),
                 BOOST_INTERVAL, BOOST_ACTIVE) {}

unique_ptr<InvisibilityGrid> InvisibilityGrid::_instance;
InvisibilityGrid* InvisibilityGrid::getInstance() {
  if (_instance == nullptr) {
    _instance = make_unique<InvisibilityGrid>();
  }
  return _instance.get();
}

InvisibilityGrid::InvisibilityGrid()
    : PotionGrid(IDS::INVISIBILITY_ID,
                 AssetManager::get_asset(INVISIBILITY_PNG), INVISIBLE_INTERVAL,
                 INVISIBLE_ACTIVE) {}
