#include "SimGame.hpp"

SimGame::SimGame() : MainGame() {}

void SimGame::runSimulation() {
  initSystems();

  initSimulation();

  startSimulation();
}

void SimGame::initSimulation() {
  robot.init(_gRenderer);
  wallGrid->init(_gRenderer);

  wallGrid->generate();

  coinGrid->init(_gRenderer);

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
    robot.move();

    preRender();

    robot.render();
    wallGrid->render();
    coinGrid->render();

    SDL_RenderPresent(_gRenderer);
  }
}