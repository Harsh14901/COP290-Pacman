#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <WallGrid.hpp>
#include <iostream>

#include "Keys.hpp"
#include "LTexture.hpp"
#include "Pacman.hpp"

using namespace std;

enum class GameState { PLAY, EXIT };

class MainGame {
 public:
  MainGame();

  void runGame();

 private:
  SDL_Window* _window;
  SDL_Surface* _screenSurface;
  SDL_Surface* _gPacman;
  SDL_Surface* _gComida;

  SDL_Renderer* _gRenderer;
  SDL_Texture* _gTexture = NULL;

  void initSystems();
  void drawInitScreen();
  void processInput();
  void gameLoop();
  bool loadMedia();
  void initCharacters();

  SDL_Texture* loadTexture(string path);

  int _screenWidth;
  int _screenHeight;

  GameState _gameState;
  Pacman _pacman;
  vector<Enemy> enemies;
};