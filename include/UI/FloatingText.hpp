#pragma once
#include "SDL2/SDL.h"
#include "Textures/LTexture.hpp"

using namespace std;


class FloatingText{

public:
    FloatingText(string text,int mPosX,int mPosY);
    void init(SDL_Renderer* _gRenderer);
    void render();
    void setPos(string text,int x,int y, bool pos);

private:
    int origY;
    int x;
    int y;
    string text;
    LTexture texture;
    SDL_Renderer* _gRenderer;
    bool disp = false;
};