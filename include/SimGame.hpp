#include "Characters/Robot.hpp"
#include "MainGame.hpp"
#include "Utils/Algorithms.hpp"

using namespace std;

class SimGame : public MainGame {
 public:
  SimGame();
  void runSimulation();

 private:
  Robot robot;
  void initSimulation();
  void startSimulation();
};
