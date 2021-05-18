#include "Characters/Robot.hpp"

Robot::Robot() : Character(IDS::ROBOT_ID) {}

bool Robot::is_terminated() {
  return direction_sequence.empty() &&
         WallGrid::getInstance()->fits_in_cell(mPosX, mPosY);
}

void Robot::load_directions(queue<Direction>& dir_seq) {
  direction_sequence = dir_seq;
}

void Robot::move() {
  if (WallGrid::getInstance()->fits_in_cell(mPosX, mPosY)) {
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