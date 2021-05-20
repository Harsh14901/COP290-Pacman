#include "AI/GhostAIEngine.hpp"

#include "Characters/Pacman.hpp"

void GhostAIEngine::init(int DOT_WIDTH, int DOT_HEIGHT, int type) {
  this->DOT_WIDTH = DOT_WIDTH;
  this->DOT_HEIGHT = DOT_HEIGHT;
  this->type = type;
}

void GhostAIEngine::update(int mPosX, int mPosY, Direction _direction,
                           EnemyState enemyState) {
  this->mPosX = mPosX;
  this->mPosY = mPosY;
  this->_direction = _direction;
  this->enemyState = enemyState;
}

int GhostAIEngine::directionToInt(Direction dir) {
  if (dir == Direction::UP) return 0;
  if (dir == Direction::RIGHT) return 1;
  if (dir == Direction::DOWN) return 2;
  return 3;
}

Direction GhostAIEngine::reverseDirectionIfNecessary(Direction dir) {
  // if(enemyState!=EnemyState::WEAK) return dir;
  // return dir==Direction::LEFT?Direction::RIGHT:
  // dir==Direction::RIGHT?Direction::LEFT:
  // dir==Direction::UP?Direction::DOWN:
  // Direction::UP;
  return dir;
}

Direction GhostAIEngine::updateDirection() {
  unordered_set<Direction> available_directions;
  auto pacman = Pacman::getInstance();

  for (int i = 0; i < 4; i++) {
    auto d = Direction(i);
    if (WallGrid::getInstance()->can_move(mPosX + DOT_WIDTH / 2,
                                          mPosY + DOT_HEIGHT / 2, d)) {
      // printf("Direction %d is available\n", i);
      available_directions.insert(d);
    }
  }
  auto directionToErase = _direction == Direction::LEFT    ? Direction::RIGHT
                          : _direction == Direction::RIGHT ? Direction::LEFT
                          : _direction == Direction::UP    ? Direction::DOWN
                                                           : Direction::UP;
  available_directions.erase(directionToErase);

  if (pacman->get_coins_collected() > 240 && !dec1) {
    pacman->MAX_VEL = pacman->MAX_VEL * 0.98;
    cout << "Reducing MAX_VEL: " << pacman->MAX_VEL << endl;
    dec1 = true;
  }

  if (available_directions.size() != 0) {
    int selection = rand() % available_directions.size();
    int i = 0;
    if (!pacman->is_invisible) {
      if (type == 0) {
        // Blinky: Target Position: Pacman
        int targetX = pacman->mPosX / 32;
        int targetY = pacman->mPosY / 32;
        return reverseDirectionIfNecessary(
            findBestDirection(targetX, targetY, available_directions));
      } else if (type == 1 && rand() % 6 != 0) {
        // Pinky: Target Position: 4 Tiles Ahead of Pacman 16% times take random
        // direction

        int offX = pacman->_direction == Direction::LEFT    ? -4
                   : pacman->_direction == Direction::RIGHT ? 4
                                                            : 0;
        int offY = pacman->_direction == Direction::UP     ? -4
                   : pacman->_direction == Direction::DOWN ? 4
                                                           : 0;
        int targetX = pacman->mPosX / 32 + offX;
        int targetY = pacman->mPosY / 32 + offY;

        return reverseDirectionIfNecessary(
            findBestDirection(targetX, targetY, available_directions));

      } else if (type == 2) {
        // Inky: Target Position: Vector extending Blinky Tiles Ahead of Pacman
        // TODO: Fix this
        int offX = pacman->_direction == Direction::LEFT    ? -6
                   : pacman->_direction == Direction::RIGHT ? 6
                                                            : 0;
        int offY = pacman->_direction == Direction::UP     ? -6
                   : pacman->_direction == Direction::DOWN ? 6
                                                           : 0;
        int int_targetX = pacman->mPosX / 32 + offX;
        int int_targetY = pacman->mPosY / 32 + offY;

      } else if (type == 3) {
        // Clyde: Target Position: If close, similar to Blinky, else in bottom
        // left half
        int targetX = 2;
        int targetY = 32;
        if (abs(pacman->mPosX / 32 - mPosX / 32) +
                (abs(pacman->mPosY / 32 - mPosY / 32)) >
            8) {
          targetX = pacman->mPosX / 32;
          targetY = pacman->mPosY / 32;
        }
        return reverseDirectionIfNecessary(
            findBestDirection(targetX, targetY, available_directions));
      }
    }

    for (auto& it : available_directions) {
      if (selection == i) {
        return it;
        break;
      }
      i++;
    }
  } else {
    // No option but to change direction
    if (mPosX % 32 == 0 && mPosY % 32 == 0) return directionToErase;
  }
}
// Direction GhostAIEngine::updateDirection(){

//   unordered_set<Direction> available_directions;
//   for (int i = 0; i < 4; i++) {
//     auto d = Direction(i);

//     if (WallGrid::getInstance()->can_move(mPosX + DOT_WIDTH / 2,
//                                           mPosY + DOT_HEIGHT / 2, d)) {
//       // printf("Direction %d is available\n", i);
//       available_directions.insert(d);
//     }
//   }
//   auto directionToErase = _direction==Direction::LEFT?Direction::RIGHT:
//   _direction==Direction::RIGHT?Direction::LEFT:
//   _direction==Direction::UP?Direction::DOWN:
//   Direction::UP;
//   available_directions.erase(directionToErase);

//   if (available_directions.size() != 0) {
//     Direction best;

//     int selection = rand() % available_directions.size();
//     int i = 0;
//     for (auto& it : available_directions) {
//       if (selection == i) {
//         best = it;
//         break;
//       }
//       i++;
//     }

//     // if(type==0){
//     //     // Blinky: Target Position: Pacman
//     //     // int targetX = pacman->mPosX / 32;
//     //     // int targetY = pacman->mPosY / 32;
//     //     best = findBestDirection(0,0,available_directions);
//     // }else{
//     //     for (auto& it : available_directions) {
//     //         if (selection == i) {
//     //             best = it;
//     //             break;
//     //         }
//     //         i++;
//     //     }
//     // }
//     return best;

//   } else {
//     // No option but to change direction
//     cout << "Strange " << endl;
//     return directionToErase;
//   }
// }

Direction GhostAIEngine::findBestDirection(
    int targetX, int targetY, unordered_set<Direction> available_directions) {
  vector<pair<Direction, int>> scores;
  int maxScore = -99;
  for (auto& it : available_directions) {
    int sc = calcDirectionScore(targetX, targetY, it);
    scores.push_back({it, sc});
    maxScore = max(sc, maxScore);
  }

  if (scores.size() == 1) return scores[0].first;

  vector<pair<Direction, int>> finalScores;
  for (auto it : scores) {
    if (it.second == maxScore && maxScore >= 0) {
      finalScores.push_back(it);
    } else if (maxScore == -1) {
      if (it.first != _direction) {
        finalScores.push_back(it);
      }
    }
  }

  return finalScores[rand() % finalScores.size()].first;
}

int GhostAIEngine::calcDirectionScore(int targetX, int targetY, Direction dir) {
  int x = mPosX / 32;
  int y = mPosY / 32;
  int diffX = targetX - x;
  int diffY = targetY - y;
  if (dir == Direction::UP) {
    return diffY < 0 ? 1 : diffY > 0 ? -1 : 0;
  } else if (dir == Direction::DOWN) {
    return diffY > 0 ? 1 : diffY < 0 ? -1 : 0;
  } else if (dir == Direction::LEFT) {
    return diffX < 0 ? 1 : diffX > 0 ? -1 : 0;
  } else if (dir == Direction::RIGHT) {
    return diffX > 0 ? 1 : diffX < 0 ? -1 : 0;
  }
}
