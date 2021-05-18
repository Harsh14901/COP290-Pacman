#include "Grids/ObjectGrid.hpp"
#include "Grids/WallGrid.hpp"
#include "Utils/Animator.hpp"

class BoostGrid : public ObjectGrid {
 public:
  BoostGrid();

  static BoostGrid* getInstance();
  void generate() override;

 private:
  static unique_ptr<BoostGrid> _instance;
  WallGrid* wallGrid;
  Animator startDelay;
  Animator potionAnimator;
};

class InvisibilityGrid : public ObjectGrid {
 public:
  InvisibilityGrid();

  static InvisibilityGrid* getInstance();
  void generate() override;

 private:
  static unique_ptr<InvisibilityGrid> _instance;
  WallGrid* wallGrid;
  Animator startDelay;
  Animator potionAnimator;
};
