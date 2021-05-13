#include "Characters/Robot.hpp"

Robot::Robot() : Character(IDS::ROBOT_ID) {}

bool Robot::fits_in_cell() {
  auto wallGrid = WallGrid::getInstance();

  auto indices = wallGrid->get_maze_point(SDL_Point{mPosX, mPosY});
  auto cell_coords = wallGrid->get_canvas_point(indices);

  return (cell_coords.x == mPosX && cell_coords.y == mPosY);
}

bool Robot::is_terminated() {
  return direction_sequence.empty() && fits_in_cell();
}

void Robot::load_directions(queue<Direction>& dir_seq) {
  direction_sequence = dir_seq;
}

void Robot::move() {
  if (fits_in_cell()) {
    change_direction(direction_sequence.front());
    direction_sequence.pop();
  }
  Character::move();
}

void Robot::render() {
  int mouth_fac = 0;
  SDL_Rect rect{138 * 4, 171 * ((2 - (int(_direction) % 2)) + mouth_fac), 138,
                171};

  // _gDotTexture.setAlpha(getInvisibleAlphaValue());
  _gDotTexture.render(mPosX, mPosY, &rect, 90 * (int(_direction) / 2));
}