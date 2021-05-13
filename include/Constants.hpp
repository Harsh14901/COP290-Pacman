#pragma once

#include <string>

using namespace std;

const static int PACMAN_DOT_WIDTH = 34;
const static int PACMAN_DOT_HEIGHT = 40;
const static int WALL_GRID_WIDTH = 32;
const static int WALL_GRID_HEIGHT = 32;
const static int PACMAN_RENDER_WIDTH = WALL_GRID_WIDTH;
const static int PACMAN_RENDER_HEIGHT = WALL_GRID_HEIGHT;
const static int WALL_GRID_ROWS = 26;
const static int WALL_GRID_COLS = 40;
const static int ENEMY_COUNT = 4;

const static int GAMEAREA_WIDTH = WALL_GRID_COLS * WALL_GRID_WIDTH;
const static int GAMEAREA_HEIGHT = WALL_GRID_ROWS * WALL_GRID_HEIGHT;  
const static int BOTTOM_BAR_HEIGHT = 100;
const static int BOTTOM_BAR_WIDTH = GAMEAREA_WIDTH;
const static int SCREEN_WIDTH = GAMEAREA_WIDTH;
const static int SCREEN_HEIGHT = GAMEAREA_HEIGHT + BOTTOM_BAR_HEIGHT;

const static int PORT = 8000;
const static int MAX_BUFF = 4096;
enum class EnemyState { NORMAL, WEAK, SUPER_ACTIVE };

enum class Direction { LEFT, RIGHT, UP, DOWN, NONE };

class IDS {
 public:
  inline const static string WALL_COLLIDER_ID = "wall";
  inline const static string PACMAN_COLLIDER_ID = "pacman";
  inline const static string ENEMY_COLLIDER_ID = "enemy";
  inline const static string COIN_COLLIDER_ID = "coins";
  inline const static string VENT_COLLIDER_ID = "vent";
  inline const static string CHERRY_COLLIDER_ID = "cherry";
  inline const static string NETWORK_ID = "networkmanager";
  inline const static string FREEZEBULLET_ID = "freezebullet";
};

enum KeyPressSurfaces {
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL
};

extern bool is_two_player;
extern double game_frame;
extern int game_frame_int;

void fatalError(string error_message);
