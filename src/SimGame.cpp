#include "SimGame.hpp"

SimGame::SimGame() : MainGame() {}

void SimGame::runSimulation() {
  drawInitScreen();

  initSimulation();

  startSimulation();
}

void SimGame::initSimulation() {
  AssetManager::init(PreferenceManager::THEME);

  wallGrid = WallGrid::getInstance();
  coinGrid = CoinGrid::getInstance();

  robot.init(_gRenderer);
  wallGrid->init(_gRenderer);
  coinGrid->init(_gRenderer);

  wallGrid->generate();

  vector<SDL_Point> tsp_points;
  auto robot_start = wallGrid->get_empty_indices();
  tsp_points.push_back(robot_start);

  robot.place(wallGrid->get_canvas_point(robot_start));

  for (int i = 0; i < TSP::MAX_SIZE - 1; i++) {
    auto p = wallGrid->get_empty_indices();
    tsp_points.push_back(p);
    coinGrid->set_object(p.x, p.y);
  }

  auto dir_seq = TSP().steinerTSP(tsp_points);
  robot.load_directions(dir_seq);
}

void SimGame::startSimulation() {
  while (!robot.is_terminated()) {
    CollisionEngine::checkCollisions();
    robot.move();
    bottomBar.update(robot.coins_collected, robot.path_length,
                     to_string(robot.path_length), robot.total_length);
    preRender();

    robot.render();
    wallGrid->render();
    coinGrid->render();
    bottomBar.render();
    SDL_RenderPresent(_gRenderer);
  }
}