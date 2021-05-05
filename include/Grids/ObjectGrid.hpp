#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <Constants.hpp>
#include <iostream>

#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "Network/NetworkManager.hpp"
#include "Textures/LTexture.hpp"

class ObjectGrid {
 public:
  const static int OBJECT_WIDTH = int(WALL_GRID_WIDTH / 1.2);
  const static int OBJECT_HEIGHT = int(WALL_GRID_HEIGHT / 1.2);

  const static int GRID_COL = WALL_GRID_COLS;
  const static int GRID_ROW = WALL_GRID_ROWS;

  // CoinGrid();
  void init(SDL_Renderer* renderer, string id, string asset_file);
  void render();

  // Set the object at position (i,j)
  void set_object(int i, int j);
  string OBJECT_ID;

  // Remove the object at position (i,j)
  void unset_object(int i, int j);

  bool can_move(int posX, int posY, Direction d);

  // Generate a objects in empty part of maze
  void generate();

  // Broadcast position of objects via NetworkManager
  // ASSUMPTION: Call in init stage, not in game loop
  // TODO: Remove this assumption
  void broadcast();

  // Recieve object packets and form a maze
  // Assumption: Call in init stage, not in game loop
  // TODO: Remove this assumption
  void packets2objects();
  int active_objects;

 protected:
  LTexture object_texture;

  // The object matrix, true indicates object is present
  bool objects[GRID_ROW][GRID_COL];

  Collider objectColliders[GRID_ROW][GRID_COL];
};