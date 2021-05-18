#include "Grids/PotionGrid.hpp"

#include "Utils/AssetManager.hpp"
using namespace std;
unique_ptr<BoostGrid> BoostGrid::_instance;
BoostGrid* BoostGrid::getInstance() {
  if (_instance == nullptr) {
    _instance = make_unique<BoostGrid>();
  }
  return _instance.get();
}

BoostGrid::BoostGrid()
    : ObjectGrid(IDS::BOOST_ID, AssetManager::get_asset(BOOST_PNG), 1.2) {
  wallGrid = WallGrid::getInstance();
  startDelay.set_duration(250);
  potionAnimator.set_duration(250);
  startDelay.start();
}

void BoostGrid::generate() {
  if (startDelay.isActive() || potionAnimator.isActive()) {
    return;
  }
  if (active_objects > 0) {
    for (int i = 0; i < GRID_ROW; i++) {
      for (int j = 0; j < GRID_COL; j++) {
        unset_object(i, j);
      }
    }
    startDelay.start();
    return;
  }
  auto empty_loc = wallGrid->get_empty_indices();
  set_object(empty_loc.x, empty_loc.y);
  potionAnimator.start();
}

unique_ptr<InvisibilityGrid> InvisibilityGrid::_instance;
InvisibilityGrid* InvisibilityGrid::getInstance() {
  if (_instance == nullptr) {
    _instance = make_unique<InvisibilityGrid>();
  }
  return _instance.get();
}

InvisibilityGrid::InvisibilityGrid()
    : ObjectGrid(IDS::INVISIBILITY_ID,
                 AssetManager::get_asset(INVISIBILITY_PNG), 1.2) {
  wallGrid = WallGrid::getInstance();
  startDelay.set_duration(600);
  potionAnimator.set_duration(400);
  startDelay.start();
}

void InvisibilityGrid::generate() {
  if (startDelay.isActive() || potionAnimator.isActive()) {
    return;
  }
  if (active_objects > 0) {
    for (int i = 0; i < GRID_ROW; i++) {
      for (int j = 0; j < GRID_COL; j++) {
        unset_object(i, j);
      }
    }
    startDelay.start();
    return;
  }
  auto empty_loc = wallGrid->get_empty_indices();
  set_object(empty_loc.x, empty_loc.y);
  potionAnimator.start();
}
