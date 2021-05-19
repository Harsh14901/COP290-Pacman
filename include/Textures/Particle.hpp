#pragma once

#include "Textures/LTexture.hpp"

using namespace std;

//Particle count
const int TOTAL_PARTICLES = 20;

class Particle
{
    public:
        Particle( int x, int y );
        void render();
        bool isDead();
        void init(SDL_Renderer* _gRenderer);
        SDL_Renderer* _gRenderer;

    private:

        int mPosX, mPosY;
        int mFrame;
        LTexture *mTexture;
        LTexture gRedTexture;
        LTexture gGreenTexture;
        LTexture gBlueTexture;
        LTexture gShimmerTexture;
};