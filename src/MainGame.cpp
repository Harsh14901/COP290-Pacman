#include "MainGame.hpp"

#include <stdio.h>	/* printf, scanf, puts, NULL */
#include <stdlib.h> /* srand, rand */
#include <time.h>	/* time */
#include <stdlib.h>

void fatalError(string error_message)
{
	cout << error_message << endl;
	int a;
	cout << "Enter any key to quit" << endl;
	cin >> a;
	exit(0);
}

MainGame::MainGame()
{
	_window = nullptr;
	_screenWidth = SCREEN_WIDTH;
	_screenHeight = SCREEN_HEIGHT;
	enemies = vector<Enemy>(ENEMY_COUNT);

	_gameState = GameState::PLAY;
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

	for (auto& enemy: enemies){
		enemy.init(_gRenderer);
	}
	

	if(system("node src/maze_generator.js > map.txt")){
		fatalError("Error Generating map");
	}

	string line;
	ifstream myfile("map.txt");
	int i = 0;
	int j = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			for (char &c : line)
			{
				if(c=='|' || c=='_'){
					WallGrid::set_wall(i,j);
				}
				j++;
			}
			cout << line << '\n';
			i++;
			j = 0;
		}
		myfile.close();
	}

	_pacman.place(WallGrid::get_empty_location());

	for (auto& enemy: enemies){
		enemy.place(WallGrid::get_empty_location());
	}
	// TODO: Change this to the algorithm.
	// The snippet below generates a random maze.
	// srand(time(NULL));
	// for (int i = 0; i < wallgrid.GRID_ROW; i++) {
	// for (int j = 0; j < wallgrid.GRID_COL; j++) {
	// if (rand() % 2 == 1) {
	// 	cout<<"Setting wall at "<< i<<" , "<<j<<endl;
	// 	wallgrid.set_wall(i, j);
	// }
	// }
	// }
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

	// SDL_BlitSurface( _gPacman, NULL, _screenSurface, NULL );

	// Fill the surface white
	// SDL_FillRect( _screenSurface, NULL, SDL_MapRGB( _screenSurface->format,
	// 0xFF, 0x0F, 0xFF ) );

	// Update the surface
	SDL_UpdateWindowSurface(_window);

	// Wait two seconds
	// SDL_Delay( 2000 );
}

void MainGame::processInput()
{
	SDL_Event evnt;
	// int key = SDL_PollEvent(evnt);
	// bool change = false;

	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;

		case SDL_MOUSEMOTION:
			// std::cout << evnt.motion.x << "," << evnt.motion.y << endl;
			break;
		case SDL_KEYDOWN:

			switch (evnt.key.keysym.sym)
			{
			case SDLK_UP:
				// cout << "Up Arrow Pressed" << endl;
				// change = true;
				break;
			default:
				break;
			}
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
	

	// if(!change){
	//     SDL_BlitSurface(_gPacman, NULL, _screenSurface, NULL);
	// }else{
	//     // SDL_BlitSurface(_gComida, NULL, _screenSurface, NULL);
	//     SDL_Rect stretchRect;
	//     stretchRect.x = 0;
	//     stretchRect.y = 0;
	//     stretchRect.w = _screenWidth;
	//     stretchRect.h = _screenHeight;

	//     SDL_BlitScaled( _gPacman, NULL, _screenSurface, &stretchRect );

	// }

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

	// SDL_Rect topRightViewport;
	// topRightViewport.x = _screenWidth/2 - 200;
	// topRightViewport.y = 0;
	// topRightViewport.w = _screenWidth / 2 + 100;
	// topRightViewport.h = _screenHeight / 2;

	// SDL_RenderSetViewport( _gRenderer, &topRightViewport );

	// //Render texture to screen
	// SDL_RenderCopy( _gRenderer, _gTexture, NULL, NULL );

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
		processInput();
		CollisionEngine::checkCollisions();
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

// MainGame::MainGame(){

// }

// void MainGame::run(){

// }

// void MainGa