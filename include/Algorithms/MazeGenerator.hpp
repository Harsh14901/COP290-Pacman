#include <bits/stdc++.h>

#include "SDL2/SDL.h"
using namespace std;
class MazeGenerator {
 public:
  enum class MazeMode { PACMAN, DFS, RANDOM };
  MazeGenerator(int N, int M);
  vector<vector<bool>> generate_maze(MazeMode mode);

 private:
  vector<vector<bool>> maze;
  void initMaze(bool initial);
  int getNodeId(int i, int j);
  SDL_Point getGridIndex(int id);
  void DFSMaze();
  void randomMaze();
  void pacmanMaze();
  int n, m;
};