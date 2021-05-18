#include "Algorithms/MazeGenerator.hpp"

MazeGenerator::MazeGenerator(int N, int M) : n(N), m(M) {}

void MazeGenerator::initMaze(bool initial) {
  maze = vector<vector<bool>>(n, vector<bool>(m, initial));
}
int MazeGenerator::getNodeId(int i, int j) { return i * m + j; }

SDL_Point MazeGenerator::getGridIndex(int id) {
  return SDL_Point{id / m, id % m};
}
vector<vector<bool>> MazeGenerator::generate_maze(MazeMode mode) {
  switch (mode) {
    case MazeMode::PACMAN:
      pacmanMaze();
      break;
    case MazeMode::RANDOM:
      randomMaze();
      break;
    case MazeMode::DFS:
      DFSMaze();
      break;
    default:
      break;
  }
  return maze;
}

void MazeGenerator::DFSMaze() {
  int size = n * m;
  vector<bool> visited(size, false);
  stack<int> nodes;

  initMaze(true);
  auto is_valid_index = [&](int num) {
    auto p = getGridIndex(num);
    int i = p.x, j = p.y;

    return i > 0 && i < n - 1 && j > 0 && j < m - 1;
  };

  auto unset_maze = [&](int num) {
    auto p = getGridIndex(num);
    maze[p.x][p.y] = 0;
  };

  auto get_unvisited = [&](int num) {
    vector<int> unvisited;
    if (!visited[num - 1]) {
      unvisited.push_back(num - 1);
    }
    if (!visited[num - m]) {
      unvisited.push_back(num - m);
    }
    if (!visited[num + 1]) {
      unvisited.push_back(num + 1);
    }
    if (!visited[num + m]) {
      unvisited.push_back(num + m);
    }
    return unvisited;
  };
  auto all_unvisited = [&](int num, int curr) {
    bool b = (get_unvisited(num).size() == 3);
    int diff = (num - curr);
    if (abs(diff) == 1) {
      b = b && !visited[num - m + diff] && !visited[num + m + diff];
    } else if (abs(diff) == m) {
      b = b && !visited[num - 1 + diff] && !visited[num + 1 + diff];
    }

    return b;
  };
  nodes.push(getNodeId(1, 1));

  while (!nodes.empty()) {
    int node_num = nodes.top();
    nodes.pop();
    visited[node_num] = true;

    unset_maze(node_num);

    vector<int> next_nodes = get_unvisited(node_num);
    vector<int> choices;
    for (auto& next : next_nodes) {
      if (is_valid_index(next) && all_unvisited(next, node_num)) {
        choices.push_back(next);
      }
    }

    if (!choices.empty()) {
      int idx = rand() % choices.size();

      nodes.push(node_num);
      nodes.push(choices[idx]);
    }
  }
}

void MazeGenerator::randomMaze() {
  initMaze(false);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (rand() % 2 == 1) {
        maze[i][j] = true;
      }
    }
  }
}

void MazeGenerator::pacmanMaze() {
  initMaze(false);
  string line;
  ifstream myfile("map.txt");
  int i = 0;
  int j = 0;
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      for (char& c : line) {
        if (c == '|' || c == '_') {
          maze[i][j] = true;
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