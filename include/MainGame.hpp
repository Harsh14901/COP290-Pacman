#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

#include <Grids/WallGrid.hpp>
#include <Grids/CoinGrid.hpp>
#include <iostream>

#include "Constants.hpp"
#include "Textures/LTexture.hpp"
#include "Characters/Pacman.hpp"
#include "Characters/Enemy.hpp"
#include "Network/NetworkManager.hpp"
#include "Grids/CherryGrid.hpp"

using namespace std;

enum class GameState { MAIN_MENU,NETWORKMENU,PLAY, EXIT };

class MainGame {
 public:
  MainGame();

  void runGame();
  void listen(Server* server);
  void connect(Client* client);


 private:
  SDL_Window* _window;
  SDL_Surface* _screenSurface;
  SDL_Surface* _gPacman;
  SDL_Surface* _gComida;

  SDL_Renderer* _gRenderer;
  SDL_Texture* _gTexture = NULL;
  Server* server = nullptr;
  Client* client = nullptr;

  void initSystems();
  void drawInitScreen();
  void processInput();
  void gameLoop();
  bool loadMedia();
  void initCharacters();
  void initNetwork();
  void testNetwork();

  SDL_Texture* loadTexture(string path);

  int _screenWidth;
  int _screenHeight;

  GameState _gameState;
  Pacman _pacman;
  // vector<Enemy> enemies;

  LTexture pacmanHeadingText;
  LTexture p1Text;
  LTexture p2Text;
  LTexture p1TextLarger;
  LTexture p2TextLarger;

  void mainMenu();
  void initMainMenuSystems();
  void mainMenuRender(int option);


  LTexture networkTextTexture;

  void networkMenu();
  void initNetworkMenu();
  void networkMenuRender();

  void renderGameEndAnimation();
  void initialiseGameEndTexture(int is_win);

  Animator gameEndAnimator = Animator(360);
  LTexture gameEndTextTexture;

};