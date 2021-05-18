#include "Constants.hpp"
#include "Grids/ObjectGrid.hpp"
#include "Grids/WallGrid.hpp"
#include "Utils/Animator.hpp"
class PotionGrid : public ObjectGrid {
 public:
  PotionGrid(string id, string asset, int interval, int active_time);
  void generate() override;

 protected:
  Animator startDelay;
  Animator potionAnimator;
};

class BoostGrid : public PotionGrid {
 public:
  BoostGrid();

  static BoostGrid* getInstance();

 private:
  static unique_ptr<BoostGrid> _instance;
};

class InvisibilityGrid : public PotionGrid {
 public:
  InvisibilityGrid();

  static InvisibilityGrid* getInstance();

 private:
  static unique_ptr<InvisibilityGrid> _instance;
};
