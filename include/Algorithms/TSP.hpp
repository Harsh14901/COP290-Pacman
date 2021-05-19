#include <bits/stdc++.h>

#include "Grids/WallGrid.hpp"

using namespace std;

class Graph {
 public:
  const static int size = WallGrid::GRID_COL * WallGrid::GRID_ROW;
  vector<vector<int>> graph;

  Graph();

  static int getNodeId(int i, int j);

  static SDL_Point getGridIndex(int id);

  void add_directions(int dst, vector<int>& preds, queue<Direction>& dir_queue);

  void bfs(int src, vector<int>& dist, vector<int>& preds);

  void makeGraph();
};

class TSP {
 public:
  const static int MAX_SIZE = 8;

  TSP();

  queue<Direction> steinerTSP(vector<SDL_Point>& locations);

 private:
  int dist_memo[1 << MAX_SIZE][MAX_SIZE];
  int next_in_path[1 << MAX_SIZE][MAX_SIZE];
  int dist_matrix[MAX_SIZE][MAX_SIZE];
  int n;
  Graph graph;

  bool is_visited(int num, int i);
  int visit(int num, int i);

  int tsp(int visited, int src);
};
