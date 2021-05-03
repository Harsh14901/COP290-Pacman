#include <CoinGrid.hpp>

const string CoinGrid::COIN_ID = COIN_COLLIDER_ID;

// WallGrid::WallGrid() { memset(walls, 0, sizeof(walls)); }
bool CoinGrid::coins[CoinGrid::GRID_ROW][CoinGrid::GRID_COL];
Collider CoinGrid::coinColliders[CoinGrid::GRID_ROW][CoinGrid::GRID_COL];
LTexture CoinGrid::coin_texture;
int CoinGrid::active_coins;

void CoinGrid::init(SDL_Renderer* renderer) {
  memset(coins, 0, sizeof(coins));
  active_coins = 0;
  coin_texture.setRenderer(renderer);
  coin_texture.loadFromFile("assets/pngs/coin2.png");
  coin_texture.set_image_dimenstions(COIN_WIDTH, COIN_HEIGHT);
}

void CoinGrid::set_coin(int i, int j) {
  if (i >= GRID_ROW || i < 0 || j >= GRID_COL || j < 0 || coins[i][j]) {
    return;
  }
  coins[i][j] = true;
  active_coins++;
  auto rect =
      SDL_Rect{j * WALL_GRID_WIDTH, i * WALL_GRID_WIDTH, WALL_GRID_WIDTH, WALL_GRID_HEIGHT};
  coinColliders[i][j] =
      Collider(COIN_ID + "_" + to_string(i) + "_" + to_string(j), rect);
  CollisionEngine::register_collider(&coinColliders[i][j]);
}

void CoinGrid::unset_coin(int i, int j) {
  if (i >= GRID_ROW || i < 0 || j >= GRID_COL || j < 0 || coins[i][j]) {
    return;
  }
  coins[i][j] = false;
  active_coins--;
  CollisionEngine::deregister_collider(&coinColliders[i][j]);
}

void CoinGrid::render() {
    // cout << "Rendering Coins" << endl;
  for (int i = 0; i < GRID_ROW; i++) {
    for (int j = 0; j < GRID_COL; j++) {
      if (coins[i][j]) {
        coin_texture.render(j * WALL_GRID_WIDTH + (WALL_GRID_WIDTH-COIN_WIDTH)/2,
                            i * WALL_GRID_HEIGHT + (WALL_GRID_HEIGHT-COIN_HEIGHT)/2);
      }
    }
  }
}

// SDL_Point CoinGrid::get_empty_location() {
//   int x = 0, y = 0;
//   while (coins[x][y]) {
//     x = rand() % GRID_ROW;
//     y = rand() % GRID_COL;
//   }
//   printf("Found empty location: (%d, %d)\n", x, y);
//   return SDL_Point{y * WALL_WIDTH, x * WALL_HEIGHT};
// }

bool CoinGrid::can_move(int posX, int posY, Direction d) {
  int row = posY / COIN_HEIGHT;
  int col = posX / COIN_WIDTH;
  bool ans = false;
  switch (d) {
    case Direction::UP:
      ans = row != 0 && !coins[row - 1][col] &&
            posX == col * COIN_WIDTH + COIN_WIDTH / 2;
      break;
    case Direction::DOWN:
      ans = row != GRID_ROW - 1 && !coins[row + 1][col] &&
            posX == col * COIN_WIDTH + COIN_WIDTH / 2;
      break;
    case Direction::RIGHT:
      ans = col != GRID_COL - 1 && !coins[row][col + 1] &&
            posY == row * COIN_HEIGHT + COIN_HEIGHT / 2;
      break;
    case Direction::LEFT:
      ans = col != 0 && !coins[row][col - 1] &&
            posY == row * COIN_HEIGHT + COIN_HEIGHT / 2;
      break;
    default:
      ans = false;
      break;
  }

  return ans;
}

void CoinGrid::generate_coins() {
//   if (system("node src/maze_generator.js > map.txt")) {
//     fatalError("Error Generating map");
//   }

  string line;
  ifstream myfile("map.txt");
  int i = 0;
  int j = 0;
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      for (char& c : line) {
        if (c == '.') {
          set_coin(i, j);
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

void CoinGrid::broadcast_coins() {
  for (int i = 0; i < GRID_ROW; i++) {
    for (int j = 0; j < GRID_COL; j++) {
      if (coins[i][j]) {
        Packet p;
        p.id = COIN_ID;
        p.posX = i;
        p.posY = j;
        NetworkManager::queue_packet(p);
      }
    }
  }
  NetworkManager::send_all();
}

