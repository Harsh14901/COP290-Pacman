#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <Constants.hpp>
#include <iostream>

#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "LTexture.hpp"
#include "Network/NetworkManager.hpp"
class WallGrid {
 public:
  // const static int GRID_COL = 28;
  // const static int GRID_ROW = 36;
  const static int WALL_WIDTH = WALL_GRID_WIDTH;
  const static int WALL_HEIGHT = WALL_GRID_HEIGHT;
  // const static int GRID_COL = SCREEN_WIDTH / WALL_WIDTH;
  // const static int GRID_ROW = SCREEN_HEIGHT / WALL_HEIGHT;
  const static int GRID_COL = WALL_GRID_COLS;
  const static int GRID_ROW = WALL_GRID_ROWS;
  const static string WALL_ID;

  // WallGrid();
  static void init(SDL_Renderer* renderer);
  static void render();

  // Set the wall at position (i,j)
  static void set_wall(int i, int j);

  // Remove the wall at position (i,j)
  static void unset_wall(int i, int j);

  // Get an empty location in the matrix
  static SDL_Point get_empty_location();

  static bool can_move(int posX, int posY, Direction d);

  // Generate a random maze
  static void generate_maze();

  // Broadcast position of walls via NetworkManager
  // ASSUMPTION: Call in init stage, not in game loop
  static void broadcast_walls();

  // Recieve wall packets and form a maze
  // Assumption: Call in init stage, not in game loop
  static void packets2maze();

 private:
  static LTexture wall_texture;

  // The wall matrix, true indicates wall is present
  static bool walls[GRID_ROW][GRID_COL];

  static int active_walls;

  static Collider wallColliders[GRID_ROW][GRID_COL];
};