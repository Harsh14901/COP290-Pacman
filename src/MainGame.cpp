#include "MainGame.hpp"

#include <stdio.h>	/* printf, scanf, puts, NULL */
#include <stdlib.h> /* srand, rand */
#include <time.h>	/* time */
#include <stdlib.h>

int game_frame;
CoinGrid coinGrid;
CherryGrid cherryGrid;

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

void MainGame::mainMenuRender(int option){

	SDL_RenderClear(_gRenderer);

	pacmanHeadingText.render(SCREEN_WIDTH/2-pacmanHeadingText.getWidth()/2,SCREEN_HEIGHT*0.12);

	if(option == 0){
		p1TextLarger.render(SCREEN_WIDTH/2-p1TextLarger.getWidth()/2,SCREEN_HEIGHT*0.52-p1TextLarger.getHeight()/2);
		p2Text.render(SCREEN_WIDTH/2-p2Text.getWidth()/2,SCREEN_HEIGHT*0.67-p2Text.getHeight()/2+p1TextLarger.getHeight()-p1Text.getHeight());
	}else{
		p1Text.render(SCREEN_WIDTH/2-p1Text.getWidth()/2,SCREEN_HEIGHT*0.5-p1Text.getHeight()/2);
		p2TextLarger.render(SCREEN_WIDTH/2-p2TextLarger.getWidth()/2,SCREEN_HEIGHT*0.65-p2TextLarger.getHeight()/2);
	}
	SDL_RenderPresent(_gRenderer);
}

void MainGame::mainMenu(){
	initMainMenuSystems();
	SDL_Event evnt;

	int menuOption = 0;
	int totalMenuOptions = 2;
	while (_gameState == GameState::PLAY){

		while (SDL_PollEvent(&evnt) && _gameState!=GameState::EXIT)
		{
			switch (evnt.type)
			{
				case SDL_QUIT:
					_gameState = GameState::EXIT;
					break;
				case SDL_KEYDOWN:
					switch(evnt.key.keysym.sym){
						case SDLK_UP:
							menuOption -=1;
							break;
						case SDLK_DOWN:
							menuOption +=1;
							break;
						default:
							cout << "Invalid Key, Play Sound" << endl;
					}
					break;
			}
		}
		mainMenuRender((menuOption+10000)%totalMenuOptions);
	}
	

}

void MainGame::initMainMenuSystems(){
	pacmanHeadingText.setRenderer(_gRenderer);
	p1Text.setRenderer(_gRenderer);
	p2Text.setRenderer(_gRenderer);
	p1TextLarger.setRenderer(_gRenderer);
	p2TextLarger.setRenderer(_gRenderer);

	pacmanHeadingText.loadFromRenderedText("Pacman",{210,255,30},TTF_OpenFont("assets/fonts/crackman.ttf",160));
	p1Text.loadFromRenderedText("1 Player",{210,255,230},TTF_OpenFont("assets/fonts/lazy.ttf",80));
	p2Text.loadFromRenderedText("2 Player",{210,255,230},TTF_OpenFont("assets/fonts/lazy.ttf",80));
	p1TextLarger.loadFromRenderedText("1 Player",{210,255,230},TTF_OpenFont("assets/fonts/lazy.ttf",120));
	p2TextLarger.loadFromRenderedText("2 Player",{210,255,230},TTF_OpenFont("assets/fonts/lazy.ttf",120));


}


void MainGame::runGame()
{
	initSystems();

	mainMenu();

	initCharacters();

	gameLoop();
}

void MainGame::initCharacters()
{
	_pacman.init(_gRenderer);
	WallGrid::init(_gRenderer);
	// CherryGrid::init(_gRenderer);
	coinGrid.init(_gRenderer,COIN_COLLIDER_ID,"assets/pngs/coin2.png");
	cherryGrid.init(_gRenderer,CHERRY_COLLIDER_ID,"assets/pngs/pac-cherry.png");
	
	int i = 0;
	for (auto& enemy: enemies){
		enemy.init(_gRenderer,i++);
	}
	
	if(server != nullptr && is_server){
		WallGrid::generate_maze();
		WallGrid::broadcast_walls();
		coinGrid.generate();
		coinGrid.broadcast();
		cherryGrid.generate();
		cherryGrid.broadcast();
	} else {
		WallGrid::packets2maze();
		coinGrid.packets2objects();
		cherryGrid.packets2objects();
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

	_window = SDL_CreateWindow("Pacman Infinity", SDL_WINDOWPOS_CENTERED,
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
	cout << "Initing Screen" << endl;
	drawInitScreen();
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
	SDL_SetRenderDrawColor(_gRenderer, 0x00, 0x00, 0x00, 0xFF);

	// Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		fatalError("SDL_image could not initialize! SDL_image Error: " +
				   string(IMG_GetError()));
	}

	if( TTF_Init() == -1 )
	{
		fatalError("SDL_ttf could not initialize! SDL_ttf Error: " +
			string(TTF_GetError()));
	}

	_gTexture = loadTexture("assets/pngs/texture.png");

	cout << "loading media" << endl;
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

	coinGrid.render();
	cherryGrid.render();

	_pacman.render();
	for (auto& enemy: enemies){
		enemy.render();
	}
	WallGrid::render();
	
	// gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth() ) / 2, ( 0.2f*SCREEN_HEIGHT - gTextTexture.getHeight() ) / 2 );

	// Update screen
	SDL_RenderPresent(_gRenderer);

	// SDL_UpdateWindowSurface(_window);
}

void MainGame::gameLoop()
{
	cout << "Starting GameLoop" << endl;
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
	// SDL_Color textColor = { 200, 0, 0 };
	// gTextTexture.setRenderer(_gRenderer);
	
	// if( !gTextTexture.loadFromRenderedText( "The quick brown fox jumps over the lazy dog", textColor,TTF_OpenFont( "assets/fonts/lazy.ttf", 28 )) )
	// {
	// 	printf( "Failed to render text texture!\n" );
	// 	return false;
	// }

    return true;
}
