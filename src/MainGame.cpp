#include "MainGame.hpp"

#include <stdio.h>  /* printf, scanf, puts, NULL */
#include <stdlib.h> /* srand, rand */
#include <stdlib.h>
#include <time.h> /* time */

#include "Characters/GhostManager.hpp"
#include "Characters/Robot.hpp"
#include "Utils/AssetManager.hpp"
#include "Utils/FrameGuider.hpp"

int game_frame_int = 0;
double game_frame = 0;

GhostManager ghostManager;
vector<Enemy*> enemies;

extern bool is_server;

void fatalError(string error_message) {
  cout << error_message << endl;
  char a;
  cout << "Enter any key to quit" << endl;
  cin >> a;
  exit(EXIT_FAILURE);
}

MainGame::MainGame() {
  _window = nullptr;
  _screenWidth = SCREEN_WIDTH;
  _screenHeight = SCREEN_HEIGHT;

  initSystems();

  Enemy::make_enemies(PreferenceManager::NUM_ENEMIES);
  enemies = Enemy::get_enemies();

  pacman = Pacman::getInstance();
  coinGrid = CoinGrid::getInstance();
  cherryGrid = CherryGrid::getInstance();
  wallGrid = WallGrid::getInstance();
  ventGrid = VentGrid::getInstance();
  boostGrid = BoostGrid::getInstance();
  invisibilityGrid = InvisibilityGrid::getInstance();

  _gameState = GameState::MAIN_MENU;
}

void MainGame::listen(Server* server) { this->server = server; }

void MainGame::connect(Client* client) { this->client = client; }

void MainGame::initNetwork() {
  if (server != nullptr) {
    server->init();
    printf("Server: Waiting for incoming connection\n");
    while (!server->is_connected()) {
      server->wait_for_connection();
    }
    NetworkManager::load_device((NetworkDevice*)server);
  } else if (client != nullptr) {
    client->init();
    printf("Client: establishing connection to server\n");
    client->connect();
    NetworkManager::load_device((NetworkDevice*)client);
  }
}

void MainGame::testNetwork() {
  int num = 10000;
  vector<string> pids = {"b", "c", "d", "e", "a"};
  vector<string> data = {"a", "b", "c", "d", "e"};

  int data_size = pids.size();

  auto send_packets = [&](NetworkDevice* device) {
    for (int i = 0; i < num; i++) {
      Packet p;
      p.id = pids[i % data_size];
      p.data = data[i % data_size];
      NetworkManager::queue_packet(p);
    }
    NetworkManager::send_all();
    cout << "[#] All sent by server. GG!" << endl;
    SDL_Delay(100);
  };

  auto recv_packets = [&](NetworkDevice* device) {
    NetworkManager::recv_all();
    int n = 0;
    for (int i = 0; i < data_size; i++) {
      vector<Packet> packets;

      NetworkManager::get_packets(pids[i], packets);
      for (auto& p : packets) {
        assert(p.id == pids[i] && p.data == data[i]);
      }

      n += packets.size();
      cout << "[#] " << n << " Hello's received by client" << endl;
    }

    assert(n == num);
    cout << "[#] All received. GG!" << endl;
  };
  if (server != nullptr) {
    send_packets(server);
    recv_packets(server);
  } else if (client != nullptr) {
    recv_packets(client);
    send_packets(client);
  }
}

void MainGame::mainMenuRender(int option) {
  SDL_RenderClear(_gRenderer);

  SDL_RenderCopy(_gRenderer, _mainMenuTexture, NULL, NULL);

  pacmanHeadingText.render(SCREEN_WIDTH / 2 - pacmanHeadingText.getWidth() / 2,
                           SCREEN_HEIGHT * 0.12);

  for(int i=0;i<numMenuButtons;i++){
    mainMenuButtons[i].set_clicked(option==i);
    mainMenuButtons[i].render();
  }

  SDL_RenderPresent(_gRenderer);
}

int MainGame::mainMenu() {
  initMainMenuSystems();
  SDL_Event evnt;

  int menuOption = 0;
  int totalMenuOptions = numMenuButtons;
  int option = 0;
  while (_gameState == GameState::MAIN_MENU) {
    while (SDL_PollEvent(&evnt) && _gameState != GameState::EXIT) {
      switch (evnt.type) {
        case SDL_QUIT:
          _gameState = GameState::EXIT;
          break;
        case SDL_KEYDOWN:
          switch (evnt.key.keysym.sym) {
            case SDLK_UP:
              menuOption -= 1;
              CommonAudios::buttonHover.play();
              break;
            case SDLK_DOWN:
              menuOption += 1;
              CommonAudios::buttonHover.play();
              break;
            case 13:
              if (menuOption!=0) CommonAudios::buttonClick.play();
              _gameState = GameState::NETWORKMENU;
            default:
              cout << "Invalid Key, Play Sound" << endl;
          }
          break;
      }
    }
    option = (menuOption + 10000) % totalMenuOptions;
    mainMenuRender(option);
  }
  pacmanMainMenuMusic.stop();
  return option;
}

void MainGame::initMainMenuSystems() {

  mainMenuButtons[0].init(0.35 * SCREEN_WIDTH, 0.40 * SCREEN_HEIGHT,
                     0.3 * SCREEN_WIDTH, 0.1 * SCREEN_HEIGHT,
                     "assets/buttons/main_menu/player1.jpeg",
                     "assets/buttons/main_menu/player1_out.jpeg");
  mainMenuButtons[1].init(0.35 * SCREEN_WIDTH, 0.55 * SCREEN_HEIGHT,
                     0.3 * SCREEN_WIDTH, 0.1 * SCREEN_HEIGHT,
                     "assets/buttons/main_menu/player2.jpeg",
                     "assets/buttons/main_menu/player2_out.jpeg");
  mainMenuButtons[2].init(0.35 * SCREEN_WIDTH, 0.70 * SCREEN_HEIGHT,
                     0.3 * SCREEN_WIDTH, 0.1 * SCREEN_HEIGHT,
                      "assets/buttons/main_menu/settings.jpeg",
                      "assets/buttons/main_menu/settings_out.jpeg");
  mainMenuButtons[3].init(0.35 * SCREEN_WIDTH, 0.85 * SCREEN_HEIGHT,
                     0.3 * SCREEN_WIDTH, 0.1 * SCREEN_HEIGHT,
                     "assets/buttons/main_menu/quit.jpeg",
                     "assets/buttons/main_menu/quit_out.jpeg");
  for(int i=0;i<numMenuButtons;i++){
    mainMenuButtons[i].setRenderer(_gRenderer);
  }


  pacmanHeadingText.setRenderer(_gRenderer);

  _mainMenuTexture = loadTexture("assets/backgrounds/mainScreenBackground.png");

  pacmanMainMenuMusic.init("assets/sounds/mainscreen_song.mp3", true);
  pacmanMainMenuMusic.play();

  pacmanHeadingText.loadFromRenderedText(
      "Pacman", {210, 255, 30}, TTF_OpenFont("assets/fonts/crackman.ttf", 160));
}

void MainGame::settingsMenu(){
  settingsScreen.init(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
  settingsScreen.setRenderer(_gRenderer);

  SDL_Event evnt;


  while (_gameState == GameState::SETTINGSMENU) {
    while (SDL_PollEvent(&evnt) && _gameState != GameState::EXIT) {
      if(evnt.key.keysym.sym==27) return;
      settingsScreen.handleEvent(evnt);
    }
    SDL_RenderClear(_gRenderer);
    settingsScreen.render();
    SDL_RenderPresent(_gRenderer);
  }
}

void MainGame::networkMenu() {
  networkTextTexture.setRenderer(_gRenderer);
  if (server != nullptr) {
    networkTextTexture.loadFromRenderedText(
        "Waiting For External Connection...", {210, 255, 230},
        TTF_OpenFont("assets/fonts/lazy.ttf", 60));
  } else if (client != nullptr) {
    networkTextTexture.loadFromRenderedText(
        "Trying to connect to server...", {210, 255, 230},
        TTF_OpenFont("assets/fonts/lazy.ttf", 70));
  }
  SDL_RenderClear(_gRenderer);
  networkTextTexture.render(
      SCREEN_WIDTH / 2 - networkTextTexture.getWidth() / 2,
      SCREEN_HEIGHT * 0.5 - networkTextTexture.getHeight() / 2);
  SDL_RenderPresent(_gRenderer);

  initNetwork();
  testNetwork();

  SDL_RenderClear(_gRenderer);
  networkTextTexture.loadFromRenderedText(
      "Connection Successfull...", {100, 255, 40},
      TTF_OpenFont("assets/fonts/lazy.ttf", 80));
  networkTextTexture.render(
      SCREEN_WIDTH / 2 - networkTextTexture.getWidth() / 2,
      SCREEN_HEIGHT * 0.5 - networkTextTexture.getHeight() / 2);
  SDL_RenderPresent(_gRenderer);
}

void MainGame::runGame() {
  cout << "Initing Screen" << endl;
  drawInitScreen();
  while(_gameState!=GameState::EXIT){
    auto option = mainMenu();

    if (option == 1) {
      networkMenu();
    } else if(option==2) {
      _gameState = GameState::SETTINGSMENU;
      settingsMenu();
      _gameState = GameState::MAIN_MENU;
      continue;
    } else if(option==3){
      _gameState = GameState::EXIT;
      continue;  
    }else {
      server = nullptr;
      client = nullptr;
      is_two_player = false;
    }

    _gameState = GameState::PLAY;
    CommonAudios::buttonStart.play();


    initCharacters();

    gameLoop();
  }
}

void MainGame::initCharacters() {
  queue<ObjectGrid*> init_grids;

  init_grids.push(wallGrid);
  init_grids.push(coinGrid);
  init_grids.push(ventGrid);
  init_grids.push(cherryGrid);
  init_grids.push(boostGrid);
  init_grids.push(invisibilityGrid);

  while (!init_grids.empty()) {
    auto grid = init_grids.front();

    grid->init(_gRenderer);
    if (server != nullptr && is_server) {
      grid->generate();
      grid->broadcast();
    } else if (client != nullptr) {
      grid->packets2objects();
    } else {
      grid->generate();
    }
    init_grids.pop();
  }

  pacman->init(_gRenderer);

  for (auto& enemy : enemies) {
    enemy->init(_gRenderer);
  }

  ghostManager.updateGhostZones();

  auto pacman_start = wallGrid->get_empty_indices();
  pacman->place(wallGrid->get_canvas_point(pacman_start));

  for (auto& enemy : enemies) {
    enemy->respawn();
  }

  BulletManager::init(_gRenderer);
}

void MainGame::initSystems() {
  cout << "Initing Assets" << endl;

  // SDL_Init(SDL_INIT_EVERYTHING);


  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    fatalError("SDL could not initialize! SDL_Error: \n" +
               string(SDL_GetError()));
  }


  _window = SDL_CreateWindow("Pacman Infinity", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, _screenWidth,
                             _screenHeight, SDL_WINDOW_OPENGL);

  if (_window == NULL) {
    fatalError("Window could not be created! SDL_Error: \n" +
               string(SDL_GetError()));
  }

  if (SDLNet_Init() < 0) {
    fatalError("SDLNet_Init Error:\n" + string(SDLNet_GetError()));
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    fatalError("SDL_mixer could not initialize! SDL_mixer Error: \n" +
               string(Mix_GetError()));
  }

  CommonAudios::initialize_sound();

  AssetManager::init(PreferenceManager::THEME);
}

SDL_Texture* MainGame::loadTexture(string path) {
  // The final texture
  SDL_Texture* newTexture = NULL;

  // Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == NULL) {
    fatalError("Unable to load image %s! SDL_image Error: %s\n" +
               string(path.c_str()) + string(IMG_GetError()));
  }
  // Create texture from surface pixels
  newTexture = SDL_CreateTextureFromSurface(_gRenderer, loadedSurface);
  if (newTexture == NULL) {
    printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(),
           SDL_GetError());
  }

  // Get rid of old loaded surface
  SDL_FreeSurface(loadedSurface);

  return newTexture;
}

void MainGame::drawInitScreen() {
  _screenSurface = SDL_GetWindowSurface(_window);

  _gRenderer = SDL_CreateRenderer(
      _window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (_gRenderer == NULL) {
    fatalError("Renderer could not be created! SDL Error: " +
               string(SDL_GetError()));
  }

  // Initialize renderer color
  SDL_SetRenderDrawColor(_gRenderer, 0x00, 0x00, 0x00, 0xFF);

  // Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    fatalError("SDL_image could not initialize! SDL_image Error: " +
               string(IMG_GetError()));
  }

  if (TTF_Init() == -1) {
    fatalError("SDL_ttf could not initialize! SDL_ttf Error: " +
               string(TTF_GetError()));
  }

  _gTexture = loadTexture("assets/backgrounds/main_space_background.jpg");

  cout << "loading media" << endl;
  if (!loadMedia()) {
    fatalError("Failed to Load Media: \n" + string(SDL_GetError()));
  }
  bottomBar.init(0, GAMEAREA_HEIGHT, BOTTOM_BAR_WIDTH, BOTTOM_BAR_HEIGHT);
  bottomBar.setRenderer(_gRenderer);
  bottomBar.initViews();

  // Update the surface
  SDL_UpdateWindowSurface(_window);
}

void MainGame::preRender() {
  SDL_SetRenderDrawColor(_gRenderer, 0xFF, 0xFF, 0x0F, 0xFF);

  SDL_RenderClear(_gRenderer);

  SDL_Rect topLeftViewport;
  topLeftViewport.x = 0;
  topLeftViewport.y = 0;
  topLeftViewport.w = _screenWidth / 1;
  topLeftViewport.h = _screenHeight / 1;

  SDL_RenderSetViewport(_gRenderer, &topLeftViewport);

  // Render texture to screen
  SDL_RenderCopy(_gRenderer, _gTexture, NULL, NULL);
}

void MainGame::processInput() {
  if (!pacman->is_dead && coinGrid->active_objects != 0) {
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt)) {
      switch (evnt.type) {
        case SDL_QUIT:
          _gameState = GameState::EXIT;
          break;
        case SDL_KEYDOWN:
          // cout << evnt.key.keysym.sym << "," << SDLK_t << endl;
          if (evnt.key.keysym.sym == SDLK_TAB) {
            Enemy::switch_active_id();
          } else if (evnt.key.keysym.sym == SDLK_t) {
            // Enemy::get_active_enemy()->shootFreezeBullet();
          }
      }

      pacman->handleEvent(evnt);
      for (auto& enemy : enemies) {
        enemy->handleEvent(evnt);
      }
    }

    pacman->move();

    for (auto& enemy : enemies) {
      enemy->move();
    }
  } else {
    if (!gameEndAnimator.isActive()) {
      if (coinGrid->active_objects == 0) {
        initialiseGameEndTexture(false);
      } else {
        initialiseGameEndTexture(false);
      }
    }
  }
  string weapon_text = (client != nullptr && is_two_player)
                           ? Enemy::get_active_enemy()->get_weapon_text()
                           : pacman->get_weapon_text();
  bottomBar.update(pacman->get_coins_collected(), pacman->get_active_points(),
                   weapon_text);
  BulletManager::update_bullets();
  boostGrid->generate();
  invisibilityGrid->generate();

  preRender();

  BulletManager::render_bullets();

  coinGrid->render();
  ventGrid->render();
  cherryGrid->render();
  boostGrid->render();
  invisibilityGrid->render();

  pacman->render();
  bottomBar.render();

  for (auto& enemy : enemies) {
    enemy->render();
  }
  wallGrid->render();

  if (pacman->is_dead) {
    if (!gameEndAnimator.isActive()) {
      initialiseGameEndTexture(false);
    }
    renderGameEndAnimation();
    _gameState = GameState::MAIN_MENU;
  }
  if (coinGrid->active_objects == 0) {
    if (!gameEndAnimator.isActive()) {
      initialiseGameEndTexture(true);
    }
    renderGameEndAnimation();
    _gameState = GameState::MAIN_MENU;
  }
  // gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth() ) / 2, (
  // 0.2f*SCREEN_HEIGHT - gTextTexture.getHeight() ) / 2 );

  // Update screen
  SDL_RenderPresent(_gRenderer);

  // SDL_UpdateWindowSurface(_window);
}

void MainGame::gameLoop() {
  cout << "Starting GameLoop" << endl;
  FrameGuider::start();
  while (_gameState == GameState::PLAY || 
         (_gameState == GameState::MAIN_MENU && gameEndAnimator.isActive())) {
    if (client != nullptr || server != nullptr) {
      NetworkManager::send_packets();
      NetworkManager::recv_packets();
    } else {
      NetworkManager::clear_all();
    }

    processInput();

    CollisionEngine::checkCollisions();

    FrameGuider::updateFrame();
    // game_frame ++;
    game_frame = FrameGuider::current_frame_double;
    game_frame_int = FrameGuider::current_frame_int;
  }
}

void MainGame::initialiseGameEndTexture(int is_win) {
  gameEndTextTexture.setRenderer(_gRenderer);
  if (is_win) {
    gameEndTextTexture.loadFromRenderedText(
        "YOU WIN!", {210, 255, 220},
        TTF_OpenFont("assets/fonts/win_font.ttf", 240));
  } else {
    pacmanDeathSound.play();
    gameEndTextTexture.loadFromRenderedText(
        "YOU LOSE!", {255, 45, 30},
        TTF_OpenFont("assets/fonts/game_over.ttf", 480));
  }
}

void MainGame::renderGameEndAnimation() {
  if (!gameEndAnimator.isActive()) {
    gameEndAnimator.start();
  }

  // cout << "Animation is happening" << endl;

  double x = gameEndAnimator.animation_progress();

  // cout << x << endl;
  double val = x > 0.33 ? 0.5 : 1.5 * x;
  gameEndTextTexture.render(
      SCREEN_WIDTH / 2 - gameEndTextTexture.getWidth() / 2,
      (1 - val) * SCREEN_HEIGHT - gameEndTextTexture.getHeight() / 2);
}

bool MainGame::loadMedia() {
  pacmanDeathSound.init("assets/sounds/pacman-death.wav", false);

  return true;
}
