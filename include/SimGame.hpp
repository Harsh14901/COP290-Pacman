#include "Algorithms/TSP.hpp"
#include "Characters/Robot.hpp"
#include "MainGame.hpp"

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
