#include <WallGrid.hpp>

const string WallGrid::WALL_ID = WALL_COLLIDER_ID;

WallGrid::WallGrid() { memset(walls, 0, sizeof(walls)); }

void WallGrid::init(SDL_Renderer* renderer) {
  wall_texture.setRenderer(renderer);
  wall_texture.loadFromFile("assets/pngs/stone_wall.png");
  wall_texture.set_image_dimenstions(WALL_WIDTH, WALL_HEIGHT);
}

void WallGrid::set_wall(int i, int j) {
  if (i >= GRID_ROW || i < 0 || j >= GRID_COL || j < 0 || walls[i][j]) {
    return;
  }
  walls[i][j] = true;
  auto rect =
      SDL_Rect{j * WALL_WIDTH, i * WALL_HEIGHT, WALL_WIDTH, WALL_HEIGHT};
  wallColliders[i][j] =
      Collider(WALL_ID + "_" + to_string(i) + "_" + to_string(j), rect);
  CollisionEngine::register_collider(&wallColliders[i][j]);
}

void WallGrid::unset_wall(int i, int j) {
  if (i >= GRID_ROW || i < 0 || j >= GRID_COL || j < 0 || !walls[i][j]) {
    return;
  }
  walls[i][j] = false;
  CollisionEngine::deregister_collider(&wallColliders[i][j]);
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

SDL_Point WallGrid::get_empty_location() {
  int x = 0, y = 0;
  while (walls[x][y]) {
    x = rand() % GRID_ROW;
    y = rand() % GRID_COL;
  }
  printf("Found empty location: (%d, %d)\n", x, y);
  return SDL_Point{y * WALL_WIDTH, x * WALL_HEIGHT};
}