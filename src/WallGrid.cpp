#include <WallGrid.hpp>

WallGrid::WallGrid() { memset(walls, 0, sizeof(walls)); }

void WallGrid::init(SDL_Renderer* renderer) {
  wall_texture.setRenderer(renderer);
  wall_texture.loadFromFile("assets/pngs/stone_wall.png");
  wall_texture.set_image_dimenstions(WALL_WIDTH, WALL_HEIGHT);
}

void WallGrid::set_wall(int i, int j) {
  if (i >= GRID_ROW || i < 0 || j >= GRID_COL || j < 0) {
    return;
  }
  walls[i][j] = true;
}

void WallGrid::unset_wall(int i, int j) {
  if (i >= GRID_ROW || i < 0 || j >= GRID_COL || j < 0) {
    return;
  }
  walls[i][j] = false;
}

void WallGrid::render() {
  for (int i = 0; i < GRID_ROW; i++) {
    for (int j = 0; j < GRID_COL; j++) {
      if (walls[i][j]) {
        wall_texture.render(j * WALL_WIDTH, i * WALL_HEIGHT);
      }
    }
  }
}