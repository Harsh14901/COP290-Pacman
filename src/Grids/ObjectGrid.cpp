#include <Grids/ObjectGrid.hpp>

void ObjectGrid::init(SDL_Renderer* renderer, string id, string asset_file) {
  memset(objects, 0, sizeof(objects));
  active_objects = 0;
  object_texture.setRenderer(renderer);
  object_texture.loadFromFile(asset_file);
  object_texture.set_image_dimenstions(OBJECT_WIDTH, OBJECT_HEIGHT);

  OBJECT_ID = id;
}

void ObjectGrid::set_object(int i, int j) {
  if (i >= GRID_ROW || i < 0 || j >= GRID_COL || j < 0 || objects[i][j]) {
    return;
  }
  objects[i][j] = true;
  active_objects++;
  auto rect = SDL_Rect{j * WALL_GRID_WIDTH, i * WALL_GRID_WIDTH,
                       WALL_GRID_WIDTH, WALL_GRID_HEIGHT};
  objectColliders[i][j] =
      Collider(OBJECT_ID + "_" + to_string(i) + "_" + to_string(j), rect);
  CollisionEngine::register_collider(&objectColliders[i][j]);
}

void ObjectGrid::unset_object(int i, int j) {
  if (i >= GRID_ROW || i < 0 || j >= GRID_COL || j < 0) {
    return;
  }
  objects[i][j] = false;
  active_objects--;
  CollisionEngine::deregister_collider(&objectColliders[i][j]);
}

void ObjectGrid::render() {
  // cout << "Rendering Objects" << endl;
  for (int i = 0; i < GRID_ROW; i++) {
    for (int j = 0; j < GRID_COL; j++) {
      if (objects[i][j]) {
        object_texture.render(
            j * WALL_GRID_WIDTH + (WALL_GRID_WIDTH - OBJECT_WIDTH) / 2,
            i * WALL_GRID_HEIGHT + (WALL_GRID_HEIGHT - OBJECT_HEIGHT) / 2);
      }
    }
  }
}

bool ObjectGrid::can_move(int posX, int posY, Direction d) {
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

void ObjectGrid::generate() {
  // SHOULD BE HANDLED BY SUBCLASS
  // TODO: TRY MAKING VIRTUAL
}

void ObjectGrid::broadcast() {
  for (int i = 0; i < GRID_ROW; i++) {
    for (int j = 0; j < GRID_COL; j++) {
      if (objects[i][j]) {
        Packet p;
        p.id = ObjectGrid::OBJECT_ID;
        p.posX = i;
        p.posY = j;
        NetworkManager::queue_packet(p);
      }
    }
  }
  NetworkManager::send_all();
}

void ObjectGrid::packets2objects() {
  NetworkManager::recv_all();
  vector<Packet> packets;
  NetworkManager::get_packets(ObjectGrid::OBJECT_ID, packets);
  for (auto& p : packets) {
    set_object(p.posX, p.posY);
  }
}