#pragma once

#include <string>

using namespace std;

// const static int SCREEN_HEIGHT = 768;
// const static int SCREEN_WIDTH = 1024;
const static int PACMAN_DOT_WIDTH = 34;
const static int PACMAN_DOT_HEIGHT = 40;
const static int WALL_GRID_WIDTH = 32;
const static int WALL_GRID_HEIGHT = 32;
const static int PACMAN_RENDER_WIDTH = WALL_GRID_WIDTH;
const static int PACMAN_RENDER_HEIGHT = WALL_GRID_HEIGHT;
const static int WALL_GRID_ROWS = 26;
const static int WALL_GRID_COLS = 40;
const static int ENEMY_COUNT = 0;
const static int SCREEN_WIDTH = WALL_GRID_COLS * WALL_GRID_WIDTH;
const static int SCREEN_HEIGHT = WALL_GRID_ROWS * WALL_GRID_HEIGHT;

const static int PORT = 8000;

enum class Direction{
  LEFT, RIGHT, UP, DOWN, NONE
};

const static string PACMAN_COLLIDER_ID = "pacman";
const static string ENEMY_COLLIDER_ID = "enemy";
const static string WALL_COLLIDER_ID = "wall";


void fatalError(string error_message);

