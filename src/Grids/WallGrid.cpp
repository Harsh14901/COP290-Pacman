#include <Grids/WallGrid.hpp>

#include "Constants.hpp"
WallGrid* WallGrid::_instance = nullptr;

WallGrid* WallGrid::getInstance() {
  if (_instance == nullptr) {
    _instance = new WallGrid();
  }
  return _instance;
}

WallGrid::WallGrid()
    : ObjectGrid(IDS::WALL_COLLIDER_ID, "assets/pngs/stone_wall.png") {}

SDL_Point WallGrid::get_empty_indices() {
  int x = 0, y = 0;
  while (objects[x][y]) {
    x = rand() % GRID_ROW;
    y = rand() % GRID_COL;
  }
  printf("Found empty location: (%d, %d)\n", x, y);
  return SDL_Point{x, y};
}
SDL_Point WallGrid::get_empty_location() {
  auto p = get_empty_indices();
  return SDL_Point{p.y * OBJECT_WIDTH, p.x * OBJECT_HEIGHT};
}

// TODO: Fix these constants
SDL_Point WallGrid::getRenderPointFromCoordi(int x,int y){
  return SDL_Point{x * 32, y * 32};
}


bool WallGrid::can_move(int posX, int posY, Direction d) {
  int row = posY / OBJECT_HEIGHT;
  int col = posX / OBJECT_WIDTH;
  bool ans = false;
  switch (d) {
    case Direction::UP:
      ans = row != 0 && !objects[row - 1][col] &&
            posX == col * OBJECT_WIDTH + OBJECT_WIDTH / 2;
      break;
    case Direction::DOWN:
      ans = row != GRID_ROW - 1 && !objects[row + 1][col] &&
            posX == col * OBJECT_WIDTH + OBJECT_WIDTH / 2;
      break;
    case Direction::RIGHT:
      ans = col != GRID_COL - 1 && !objects[row][col + 1] &&
            posY == row * OBJECT_HEIGHT + OBJECT_HEIGHT / 2;
      break;
    case Direction::LEFT:
      ans = col != 0 && !objects[row][col - 1] &&
            posY == row * OBJECT_HEIGHT + OBJECT_HEIGHT / 2;
      break;
    default:
      ans = false;
      break;
  }

  return ans;
}

void WallGrid::generate() {
  if (system("node src/maze_generator.js > map.txt")) {
    fatalError("Error Generating map");
  }

  string line;
  ifstream myfile("map.txt");
  int i = 0;
  int j = 0;
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      for (char& c : line) {
        if (c == '|' || c == '_' ) {
          set_object(i, j);
        }
        j++;
      }
      cout << line << '\n';
      i++;
      j = 0;
    }
    myfile.close();
  }
}
