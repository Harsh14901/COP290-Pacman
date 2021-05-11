#include <bits/stdc++.h>

#include "Grids/WallGrid.hpp"

using namespace std;

class Graph {
 public:
  const static int size = WallGrid::GRID_COL * WallGrid::GRID_ROW;
  vector<vector<int>> graph;

  Graph() {
    graph.resize(size);
    makeGraph();
  }

  static int getNodeId(int i, int j) { return i * WallGrid::GRID_COL + j; }

  static SDL_Point getGridIndex(int id) {
    return SDL_Point{id / WallGrid::GRID_COL, id % WallGrid::GRID_COL};
  }

  void bfs(int src, vector<int>& dist, vector<int>& preds) {
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

  void makeGraph() {
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
};

class TSP {
 public:
  const static int MAX_SIZE = 8;

  TSP() {
    memset(dist_memo, -1, sizeof(dist_memo));
    memset(next_in_path, -1, sizeof(next_in_path));
    memset(dist_matrix, -1, sizeof(dist_matrix));
  }

  vector<SDL_Point> steinerTSP(vector<SDL_Point> locations) {
    vector<int> nodes;
    for (auto& p : locations) {
      nodes.push_back(Graph::getNodeId(p.x, p.y));
    }

    n = nodes.size();
    assert(n <= MAX_SIZE);

    for (int i = 0; i < n; i++) {
      vector<int> dist, preds;
      graph.bfs(nodes[i], dist, preds);
      for (int j = 0; j < n; j++) {
        dist_matrix[i][j] = dist[nodes[j]];
      }
    }

    tsp(0, 0);

    vector<SDL_Point> path;
    int next = 0, visited = 0;
    for (int i = 0; i <= n; i++) {
      // cout<<"next: "<<next<<endl;
      next = (next == -1) ? 0 : next;
      path.push_back(Graph::getGridIndex(nodes[next]));
      int new_visited = visit(visited, next);
      next = next_in_path[visited][next];
      visited = new_visited;
    }
    return path;
  }

 private:
  int dist_memo[1 << MAX_SIZE][MAX_SIZE];
  int next_in_path[1 << MAX_SIZE][MAX_SIZE];
  int dist_matrix[MAX_SIZE][MAX_SIZE];
  int n;
  Graph graph;

  bool is_visited(int num, int i) { return (num & (1 << i)) != 0; }
  int visit(int num, int i) { return num | (1 << i); }

  int tsp(int visited, int src) {
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
};
