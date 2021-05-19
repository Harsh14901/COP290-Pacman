#include <Grids/WallGrid.hpp>

#include "Algorithms/MazeGenerator.hpp"
#include "Constants.hpp"
#include "Utils/AssetManager.hpp"
unique_ptr<WallGrid> WallGrid::_instance;
WallGrid* WallGrid::getInstance() {
  if (_instance == nullptr) {
    _instance = make_unique<WallGrid>();
  }
  return _instance.get();
}

WallGrid::WallGrid()
    : ObjectGrid(IDS::WALL_COLLIDER_ID, AssetManager::get_asset(ThemeAssets::WALL_PNG)) {}

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
  return get_canvas_point(get_empty_indices());
}

// TODO: Fix these constants
SDL_Point WallGrid::getRenderPointFromCoordi(int x, int y) {
  return SDL_Point{x * OBJECT_WIDTH, y * OBJECT_HEIGHT};
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

bool WallGrid::is_empty(int i, int j) { return !objects[i][j]; }

void WallGrid::generate() {
  if (system("node src/maze_generator.js > map.txt")) {
    fatalError("Error Generating map");
  }
  auto mazeGen = MazeGenerator(GRID_ROW, GRID_COL);
  auto maze = mazeGen.generate_maze(MazeGenerator::MazeMode::PACMAN);

  for (int i = 0; i < GRID_ROW; i++) {
    for (int j = 0; j < GRID_COL; j++) {
      if (maze[i][j]) {
        set_object(i, j);
      }
    }
  }
}

bool WallGrid::fits_in_cell(int x, int y) {
  auto indices = get_maze_point(SDL_Point{x, y});
  auto cell_coords = get_canvas_point(indices);

  return (cell_coords.x == x && cell_coords.y == y);
}