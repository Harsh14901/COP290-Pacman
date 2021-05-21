#include "Characters/Robot.hpp"

#include "Grids/CoinGrid.hpp"

Robot::Robot() : Character(IDS::ROBOT_ID) {}

bool Robot::is_terminated() {
  return direction_sequence.empty() &&
         WallGrid::getInstance()->fits_in_cell(mPosX, mPosY);
}

void Robot::load_directions(queue<Direction>& dir_seq) {
  direction_sequence = dir_seq;
  total_length = dir_seq.size();
}

void Robot::init_targets() {
  add_target(IDS::COIN_COLLIDER_ID);
  Character::init_targets();
}

void Robot::move() {
  if (WallGrid::getInstance()->fits_in_cell(mPosX, mPosY)) {
    change_direction(direction_sequence.front());
    direction_sequence.pop();
    path_length++;
  }
  handle_collision();
  Character::move();
}

void Robot::render() {
  int mouth_fac = 0;
  SDL_Rect rect{0, 171 * ((1 - (int(_direction) % 2)) + mouth_fac), 162, 171};

  _gDotTexture.render(mPosX, mPosY, &rect, 90 * (int(_direction) / 2));
}
void Robot::target_hit(string target_id, Collider* collider) {
  if (collider != nullptr) {
    auto temp = extractIntegerWords(collider->id);
    if (target_id == IDS::COIN_COLLIDER_ID) {
      if (temp.size() == 2) {
        CoinGrid::getInstance()->unset_object(temp[0], temp[1]);
        coins_collected++;
      }
    }
  }
}

Robot::~Robot() {}
