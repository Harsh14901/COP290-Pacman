#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

#include <Grids/CoinGrid.hpp>
#include <Grids/WallGrid.hpp>
#include <iostream>

#include "Characters/Enemy.hpp"
#include "Characters/Pacman.hpp"
#include "Constants.hpp"
#include "Grids/CherryGrid.hpp"
#include "Grids/PotionGrid.hpp"
#include "Grids/VentGrid.hpp"
#include "Network/NetworkManager.hpp"
#include "Screens/SettingsScreen.hpp"
#include "Textures/LTexture.hpp"
#include "UI/BottomBar.hpp"
#include "UI/ImageButton.hpp"
#include "Utils/AudioAsset.hpp"
#include "Utils/CommonAudios.hpp"

using namespace std;

enum class GameState { MAIN_MENU, NETWORKMENU, PLAY, SETTINGSMENU, EXIT };

class MainGame {
 public:
  MainGame();

  void runGame();
  void runSimulation();
  void listen(Server* server);
  void connect(Client* client);

 protected:
  void drawInitScreen();

  void initSystems();
  void preRender();
  void renderAll();
  SDL_Window* _window;
  SDL_Surface* _screenSurface;
  SDL_Surface* _gPacman;
  SDL_Surface* _gComida;

  SDL_Renderer* _gRenderer;
  SDL_Texture* _gTexture = NULL;
  SDL_Texture* _mainMenuTexture = NULL;

  Server* server = nullptr;
  Client* client = nullptr;

  int _screenWidth;
  int _screenHeight;

  GameState _gameState;

  LTexture pacmanHeadingText;
  LTexture p1Text;
  LTexture p2Text;
  LTexture p1TextLarger;
  LTexture p2TextLarger;
  ImageButton player1Button;
  ImageButton player2Button;
  ImageButton settingsButton;
  ImageButton quitButton;

  const static int numMenuButtons = 4;
  ImageButton mainMenuButtons[numMenuButtons];

  LTexture networkTextTexture;

  Animator gameEndAnimator = Animator(360);
  LTexture gameEndTextTexture;

  AudioAsset pacmanDeathSound;
  AudioAsset pacmanMainMenuMusic;

  BottomBar bottomBar;

  WallGrid* wallGrid;
  CoinGrid* coinGrid;
  CherryGrid* cherryGrid;
  VentGrid* ventGrid;
  BoostGrid* boostGrid;
  InvisibilityGrid* invisibilityGrid;
  Pacman* pacman;

 private:
  void processInput();
  void gameLoop();
  bool loadMedia();
  void initCharacters();
  void initNetwork();
  void testNetwork();

  bool game_terminated();
  void clear_resources();

  SDL_Texture* loadTexture(string path);

  // Returns the option selected
  int mainMenu();
  void initMainMenuSystems();
  void mainMenuRender(int option);

  void networkMenu();
  void initNetworkMenu();
  void networkMenuRender();

  void settingsMenu();
  SettingsScreen settingsScreen;

  void renderGameEndAnimation();
  void initialiseGameEndTexture(bool is_win);
};