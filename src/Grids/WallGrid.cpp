#include <Grids/WallGrid.hpp>

const string WallGrid::WALL_ID = WALL_COLLIDER_ID;

// WallGrid::WallGrid() { memset(walls, 0, sizeof(walls)); }
bool WallGrid::walls[WallGrid::GRID_ROW][WallGrid::GRID_COL];
Collider WallGrid::wallColliders[WallGrid::GRID_ROW][WallGrid::GRID_COL];
LTexture WallGrid::wall_texture;
int WallGrid::active_walls;

void WallGrid::init(SDL_Renderer* renderer) {
  memset(walls, 0, sizeof(walls));
  active_walls = 0;
  wall_texture.setRenderer(renderer);
  wall_texture.loadFromFile("assets/pngs/stone_wall.png");
  wall_texture.set_image_dimenstions(WALL_WIDTH, WALL_HEIGHT);
}

void WallGrid::set_wall(int i, int j) {
  if (i >= GRID_ROW || i < 0 || j >= GRID_COL || j < 0 || walls[i][j]) {
    return;
  }
  walls[i][j] = true;
  active_walls++;
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
  active_walls--;
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

bool WallGrid::can_move(int posX, int posY, Direction d) {
  int row = posY / WALL_HEIGHT;
  int col = posX / WALL_WIDTH;
  bool ans = false;
  switch (d) {
    case Direction::UP:
      ans = row != 0 && !walls[row - 1][col] &&
            posX == col * WALL_WIDTH + WALL_WIDTH / 2;
      break;
    case Direction::DOWN:
      ans = row != GRID_ROW - 1 && !walls[row + 1][col] &&
            posX == col * WALL_WIDTH + WALL_WIDTH / 2;
      break;
    case Direction::RIGHT:
      ans = col != GRID_COL - 1 && !walls[row][col + 1] &&
            posY == row * WALL_HEIGHT + WALL_HEIGHT / 2;
      break;
    case Direction::LEFT:
      ans = col != 0 && !walls[row][col - 1] &&
            posY == row * WALL_HEIGHT + WALL_HEIGHT / 2;
      break;
    default:
      ans = false;
      break;
  }

  return ans;
}

void WallGrid::generate_maze() {
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
        if (c == '|' || c == '_') {
          set_wall(i, j);
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

void WallGrid::broadcast_walls() {
  for (int i = 0; i < GRID_ROW; i++) {
    for (int j = 0; j < GRID_COL; j++) {
      if (walls[i][j]) {
        Packet p;
        p.id = WALL_ID;
        p.posX = i;
        p.posY = j;
        NetworkManager::queue_packet(p);
      }
    }
  }
  NetworkManager::send_all();
}

void WallGrid::packets2maze() {
  NetworkManager::recv_all();
  vector<Packet> packets;
  NetworkManager::get_packets(WALL_ID, packets);
  for (auto& p : packets) {
    set_wall(p.posX, p.posY);
  }
}