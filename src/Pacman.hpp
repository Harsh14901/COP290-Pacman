#pragma once

#include <iostream>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "LTexture.hpp"

class Pacman
{
    public:
        //The dimensions of the dot
        static const int DOT_WIDTH = 34;
        static const int DOT_HEIGHT = 40;

        //Maximum axis velocity of the dot
        static const int DOT_VEL = 8;

        //Initializes the variables
        Pacman();
        void init(SDL_Renderer* renderer);


        //Takes key presses and adjusts the dot's velocity
        void handleEvent( SDL_Event& e );

        //Moves the dot
        void move();

        //Shows the dot on the screen
        void render();

    private:
        //The X and Y offsets of the dot
        int mPosX, mPosY;

        //The velocity of the dot
        int mVelX, mVelY;
        LTexture _gDotTexture;
        int _direction = 0;

};