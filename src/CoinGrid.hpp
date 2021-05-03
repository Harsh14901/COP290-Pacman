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

class CoinGrid {
 public:
  const static int COIN_WIDTH = int(WALL_GRID_WIDTH / 1.2);
  const static int COIN_HEIGHT = int(WALL_GRID_HEIGHT / 1.2);

  const static int GRID_COL = WALL_GRID_COLS;
  const static int GRID_ROW = WALL_GRID_ROWS;

  const static string COIN_ID;

  // CoinGrid();
  static void init(SDL_Renderer* renderer);
  static void render();

  // Set the coin at position (i,j)
  static void set_coin(int i, int j);

  // Remove the coin at position (i,j)
  static void unset_coin(int i, int j);

  static bool can_move(int posX, int posY, Direction d);

  // Generate a coins in empty part of maze
  static void generate_coins();

  // Broadcast position of coins via NetworkManager
  // ASSUMPTION: Call in init stage, not in game loop
  // TODO: Remove this assumption
  static void broadcast_coins();

  // Recieve coin packets and form a maze
  // Assumption: Call in init stage, not in game loop
  // TODO: Remove this assumption
  static void packets2coins();

 private:
  static LTexture coin_texture;

  // The coin matrix, true indicates coin is present
  static bool coins[GRID_ROW][GRID_COL];

  static int active_coins;

  static Collider coinColliders[GRID_ROW][GRID_COL];
};