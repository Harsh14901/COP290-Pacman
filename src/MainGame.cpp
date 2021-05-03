#include "MainGame.hpp"

#include <stdio.h>	/* printf, scanf, puts, NULL */
#include <stdlib.h> /* srand, rand */
#include <time.h>	/* time */
#include <stdlib.h>

int game_frame;

void fatalError(string error_message)
{
	cout << error_message << endl;
	char a;
	cout << "Enter any key to quit" << endl;
	cin >> a;
	exit(EXIT_FAILURE);
}

MainGame::MainGame()
{
	_window = nullptr;
	_screenWidth = SCREEN_WIDTH;
	_screenHeight = SCREEN_HEIGHT;
	enemies = vector<Enemy>(ENEMY_COUNT);

	_gameState = GameState::PLAY;
}

void MainGame::listen(Server* server){
	this->server = server;
}

void MainGame::connect(Client* client){
	this->client = client;
}

void MainGame::initNetwork(){
	if(server != nullptr){
		server->init();
		printf("Server: Waiting for incoming connection\n");
		while(!server->is_connected()){
			server->wait_for_connection();
		}
		NetworkManager::load_device((NetworkDevice *)server);
	} else if(client != nullptr){
		client->init();
		printf("Client: establishing connection to server\n");
		client->connect();
		NetworkManager::load_device((NetworkDevice *)client);
	}
}

void MainGame::testNetwork(){
	int num = 10000;
	vector<string> pids = {"b", "c", "d", "e", "a"};
	vector<string> data = {"a", "b", "c", "d", "e"};

	int data_size = pids.size();

	auto send_packets = [&](NetworkDevice* device){
		for (int i = 0; i < num; i++)
		{	
			Packet p;
			p.id = pids[i%data_size];
			p.data = data[i% data_size];
			NetworkManager::queue_packet(p);
		}
		NetworkManager::send_all();
		cout<<"[#] All sent by server. GG!"<<endl;
		SDL_Delay(100);
	};

	auto recv_packets = [&](NetworkDevice* device){
		NetworkManager::recv_all();
		int n = 0;
		for (int i = 0; i < data_size; i++){
			vector<Packet> packets;

			NetworkManager::get_packets(pids[i], packets);
			for(auto&p :packets){
				assert(p.id == pids[i] && p.data == data[i]);
			}
			
			n += packets.size();
			cout<<"[#] "<< n <<" Hello's received by client"<<endl;

		}

		assert(n == num);
		cout<<"[#] All received. GG!"<<endl;
	};
	if(server != nullptr){
		send_packets(server);
		recv_packets(server);
	} else if(client != nullptr){
		recv_packets(client);
		send_packets(client);
	}
}

void MainGame::runGame()
{
	initSystems();

	gameLoop();
}

void MainGame::initCharacters()
{
	_pacman.init(_gRenderer);
	WallGrid::init(_gRenderer);
	CoinGrid::init(_gRenderer);

	int i = 0;
	for (auto& enemy: enemies){
		enemy.init(_gRenderer,i++);
	}
	
	if(server != nullptr && is_server){
		WallGrid::generate_maze();
		WallGrid::broadcast_walls();
		CoinGrid::generate_coins();
		CoinGrid::broadcast_coins();
	} else {
		WallGrid::packets2maze();
		CoinGrid::packets2coins();
	}
	
	_pacman.place(WallGrid::get_empty_location());

	for (auto& enemy: enemies){
		enemy.place(WallGrid::get_empty_location());
	}

}

void MainGame::initSystems()
{
	// SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Surface *screenSurface = NULL;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fatalError("SDL could not initialize! SDL_Error: \n" +
				   string(SDL_GetError()));
	}

	_window = SDL_CreateWindow("Graphics Engine", SDL_WINDOWPOS_CENTERED,
							   SDL_WINDOWPOS_CENTERED, _screenWidth,
							   _screenHeight, SDL_WINDOW_OPENGL);

	if (_window == NULL)
	{
		fatalError("Window could not be created! SDL_Error: \n" +
				   string(SDL_GetError()));
	}

	if (SDLNet_Init() < 0) {
    fatalError("SDLNet_Init Error:\n" + string(SDLNet_GetError()));
  }

	initNetwork();
	testNetwork();
	drawInitScreen();
	initCharacters();
}

SDL_Texture *MainGame::loadTexture(string path)
{
	// The final texture
	SDL_Texture *newTexture = NULL;

	// Load image at specified path
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		fatalError("Unable to load image %s! SDL_image Error: %s\n" +
				   string(path.c_str()) + string(IMG_GetError()));
	}
	// Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(_gRenderer, loadedSurface);
	if (newTexture == NULL)
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(),
			   SDL_GetError());
	}

	// Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	return newTexture;
}

void MainGame::drawInitScreen()
{
	_screenSurface = SDL_GetWindowSurface(_window);

	_gRenderer = SDL_CreateRenderer(
		_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (_gRenderer == NULL)
	{
		fatalError("Renderer could not be created! SDL Error: " +
				   string(SDL_GetError()));
	}

	// Initialize renderer color
	SDL_SetRenderDrawColor(_gRenderer, 0xFF, 0xFF, 0x0F, 0xFF);

	// Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		fatalError("SDL_image could not initialize! SDL_image Error: " +
				   string(IMG_GetError()));
	}

	_gTexture = loadTexture("assets/pngs/texture.png");

	if (!loadMedia())
	{
		fatalError("Failed to Load Media: \n" + string(SDL_GetError()));
	}

	// Update the surface
	SDL_UpdateWindowSurface(_window);

}

void MainGame::processInput()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		}

		_pacman.handleEvent(evnt);
		for (auto& enemy: enemies){
			enemy.handleEvent(evnt);
		}
	}

	_pacman.move();

	for (auto& enemy: enemies){
		enemy.move();
	}
	
	SDL_SetRenderDrawColor(_gRenderer, 0xFF, 0xFF, 0x0F, 0xFF);

	SDL_RenderClear(_gRenderer);

	SDL_Rect topLeftViewport;
	topLeftViewport.x = 0;
	topLeftViewport.y = 0;
	topLeftViewport.w = _screenWidth / 1;
	topLeftViewport.h = _screenHeight / 1.01;

	SDL_RenderSetViewport(_gRenderer, &topLeftViewport);

	// Render texture to screen
	SDL_RenderCopy(_gRenderer, _gTexture, NULL, NULL);

	CoinGrid::render();

	_pacman.render();
	for (auto& enemy: enemies){
		enemy.render();
	}
	WallGrid::render();

	// Update screen
	SDL_RenderPresent(_gRenderer);

	// SDL_UpdateWindowSurface(_window);
}

void MainGame::gameLoop()
{
	while (_gameState == GameState::PLAY)
	{
		NetworkManager::send_packets();
		NetworkManager::recv_packets();

		processInput();
		CollisionEngine::checkCollisions();
		game_frame++;
	}
}

bool MainGame::loadMedia()
{
	// Loading success flag
	bool success = true;

	// Load splash image
	auto _gPacman_temp = SDL_LoadBMP("assets/bitmaps/pacman.bmp");
	// _gComida = SDL_LoadBMP("assets/comida1.bmp");

	if (_gPacman_temp == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n",
			   "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError());
		success = false;
	}

	_gPacman = SDL_ConvertSurface(_gPacman_temp, _screenSurface->format, 0);

	SDL_FreeSurface(_gPacman_temp);

	return success;
}
