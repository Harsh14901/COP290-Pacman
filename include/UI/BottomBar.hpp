#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include "Textures/LTexture.hpp"
#include "UI/ProgressBar.h"
#include "utils.hpp"


using namespace std;

class BottomBar{
public:
    BottomBar(int x,int y,int w,int h);
    void init(int x,int y,int w,int h);
    void setRenderer(SDL_Renderer* _gRenderer);

    void render();
    void initViews();
    void update(int coins,int activePoints);

private:
    int x;
    int y;
    int h;
    int w;
    SDL_Renderer* _gRenderer;

    LTexture CoinImage;
    LTexture CoinsCollectedText;

    int coinsCollected = 0;
    int prevCoinsCollected = 0;
    int activePoints = 0;

    ProgressBar pacmanPointBar;

};