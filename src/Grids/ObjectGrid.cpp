#include <Grids/ObjectGrid.hpp>

ObjectGrid::ObjectGrid(string id, string asset_file, double scale)
    : OBJECT_ID(id),
      active_objects(0),
      OBJECT_WIDTH(int(WALL_GRID_WIDTH / scale)),
      OBJECT_HEIGHT(int(WALL_GRID_HEIGHT / scale)),
      asset_file(asset_file) {
  memset(objects, 0, sizeof(objects));
}

void ObjectGrid::init(SDL_Renderer* renderer) {
  object_texture.setRenderer(renderer);
  object_texture.loadFromFile(asset_file);
  object_texture.set_image_dimenstions(OBJECT_WIDTH, OBJECT_HEIGHT);
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

bool ObjectGrid::is_set(int i, int j) {
  if (i >= GRID_ROW || i < 0 || j >= GRID_COL || j < 0) {
    return false;
  }
  return objects[i][j] == 1;
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

SDL_Point ObjectGrid::get_maze_point(SDL_Point canvas_point) {
  return SDL_Point{canvas_point.y / OBJECT_HEIGHT,
                   canvas_point.x / OBJECT_WIDTH};
}

SDL_Point ObjectGrid::get_canvas_point(SDL_Point maze_indices) {
  return SDL_Point{maze_indices.y * OBJECT_WIDTH,
                   maze_indices.x * OBJECT_HEIGHT};
}

void ObjectGrid::broadcast() {
  for (int i = 0; i < GRID_ROW; i++) {
    for (int j = 0; j < GRID_COL; j++) {
      if (objects[i][j]) {
        Packet p;
        p.id = OBJECT_ID;
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
  NetworkManager::get_packets(OBJECT_ID, packets);
  for (auto& p : packets) {
    set_object(p.posX, p.posY);
  }
}

void ObjectGrid::generate() {}

ObjectGrid::~ObjectGrid() {}