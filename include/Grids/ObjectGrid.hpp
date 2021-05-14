#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "Network/NetworkManager.hpp"
#include "Textures/LTexture.hpp"

class ObjectGrid {
 public:
  const static int GRID_COL = WALL_GRID_COLS;
  const static int GRID_ROW = WALL_GRID_ROWS;

  string OBJECT_ID;
  int active_objects;
  int OBJECT_WIDTH;
  int OBJECT_HEIGHT;

  void init(SDL_Renderer* renderer);
  void render();

  // Set the object at position (i,j)
  void set_object(int i, int j);

  // Remove the object at position (i,j)
  void unset_object(int i, int j);

  // Returns if the object is set at position (i,j)
  bool is_set(int i, int j);

  bool can_move(int posX, int posY, Direction d);

  // Generate a objects in empty part of maze
  virtual void generate();

  // Broadcast position of objects via NetworkManager
  // ASSUMPTION: Call in init stage, not in game loop
  // TODO: Remove this assumption
  void broadcast();

  // Recieve object packets and form a maze
  // Assumption: Call in init stage, not in game loop
  // TODO: Remove this assumption
  void packets2objects();

  SDL_Point get_maze_point(SDL_Point canvas_point);
  SDL_Point get_canvas_point(SDL_Point maze_indices);

  virtual ~ObjectGrid();

 protected:
  ObjectGrid(string id, string asset_file, double scale = 1.0);
  LTexture object_texture;

  // The object matrix, true indicates object is present
  bool objects[GRID_ROW][GRID_COL];
  string asset_file;

  Collider objectColliders[GRID_ROW][GRID_COL];
};