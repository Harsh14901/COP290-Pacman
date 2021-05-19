#include "Algorithms/TSP.hpp"

Graph::Graph() {
  graph.resize(size);
  makeGraph();
}

int Graph::getNodeId(int i, int j) { return i * WallGrid::GRID_COL + j; }

SDL_Point Graph::getGridIndex(int id) {
  return SDL_Point{id / WallGrid::GRID_COL, id % WallGrid::GRID_COL};
}

void Graph::add_directions(int dst, vector<int>& preds,
                           queue<Direction>& dir_queue) {
  stack<Direction> directions;

  int curr = dst;
  while (preds[curr] != -1) {
    auto pred = preds[curr];
    Direction d;
    if (pred == curr - 1) {
      d = Direction::RIGHT;
    } else if (pred == curr + 1) {
      d = Direction::LEFT;
    } else if (pred < curr) {
      d = Direction::DOWN;
    } else {
      d = Direction::UP;
    }

    directions.push(d);
    curr = pred;
  }

  while (!directions.empty()) {
    auto d = directions.top();
    dir_queue.push(d);
    directions.pop();
  }
}

void Graph::bfs(int src, vector<int>& dist, vector<int>& preds) {
  dist = vector<int>(size, -1);
  preds = vector<int>(size, -1);

  queue<int> nodes;

  dist[src] = 0;
  nodes.push(src);

  while (!nodes.empty()) {
    int node = nodes.front();
    nodes.pop();

    for (auto& neighbor : graph[node]) {
      if (dist[neighbor] == -1) {
        dist[neighbor] = dist[node] + 1;
        preds[neighbor] = node;
        nodes.push(neighbor);
      }
    }
  }
}

void Graph::makeGraph() {
  auto wallGrid = WallGrid::getInstance();

  for (int i = 0; i < WallGrid::GRID_ROW; i++) {
    for (int j = 0; j < WallGrid::GRID_COL; j++) {
      if (!wallGrid->is_set(i, j)) {
        auto id = getNodeId(i, j);
        if (!wallGrid->is_set(i + 1, j)) {
          graph[id].push_back(getNodeId(i + 1, j));
        }
        if (!wallGrid->is_set(i - 1, j)) {
          graph[id].push_back(getNodeId(i - 1, j));
        }
        if (!wallGrid->is_set(i, j + 1)) {
          graph[id].push_back(getNodeId(i, j + 1));
        }
        if (!wallGrid->is_set(i, j - 1)) {
          graph[id].push_back(getNodeId(i, j - 1));
        }
      }
    }
  }
}

TSP::TSP() {
  memset(dist_memo, -1, sizeof(dist_memo));
  memset(next_in_path, -1, sizeof(next_in_path));
  memset(dist_matrix, -1, sizeof(dist_matrix));
}

queue<Direction> TSP::steinerTSP(vector<SDL_Point>& locations) {
  vector<int> nodes;
  for (auto& p : locations) {
    nodes.push_back(Graph::getNodeId(p.x, p.y));
  }

  n = nodes.size();
  assert(n <= MAX_SIZE);

  vector<vector<int>> preds(n);

  for (int i = 0; i < n; i++) {
    vector<int> dist;
    graph.bfs(nodes[i], dist, preds[i]);
    for (int j = 0; j < n; j++) {
      dist_matrix[i][j] = dist[nodes[j]];
    }
  }

  tsp(0, 0);

  queue<Direction> direction_sequence;
  int curr = 0, visited = 0;
  for (int i = 0; i < n; i++) {
    auto t = Graph::getGridIndex(nodes[curr]);
    printf("TSP solution: (%d, %d)\n", t.x, t.y);

    int next = next_in_path[visited][curr];
    next = (next == -1) ? 0 : next;
    graph.add_directions(nodes[next], preds[curr], direction_sequence);

    visited = visit(visited, curr);
    curr = next;
  }
  return direction_sequence;
}

bool TSP::is_visited(int num, int i) { return (num & (1 << i)) != 0; }
int TSP::visit(int num, int i) { return num | (1 << i); }

int TSP::tsp(int visited, int src) {
  if (dist_memo[visited][src] != -1) {
    return dist_memo[visited][src];
  }
  int new_visited = visit(visited, src);
  int ans = INT_MAX;
  int next = -1;
  for (int i = 0; i < n; i++) {
    if (dist_matrix[src][i] != -1 && !is_visited(new_visited, i)) {
      int temp = tsp(new_visited, i);
      int new_dist = dist_matrix[src][i] + temp;
      if (new_dist < ans) {
        ans = new_dist;
        next = i;
      }
    }
  }
  if (new_visited == (1 << n) - 1) {
    ans = dist_matrix[src][0];
  }
  dist_memo[visited][src] = ans;
  next_in_path[visited][src] = next;
  return dist_memo[visited][src];
}
