#pragma once

#include <iostream>
#include <SDL2/SDL.h>

class ProgressBar{

public:
   ProgressBar();
   ProgressBar(int x,int y,int w, int h,SDL_Color foreground,SDL_Color background);
   void init(int x,int y,int w, int h,SDL_Color foreground,SDL_Color background);
   void render(float percent);
   void setRenderer( SDL_Renderer* _gRenderer);
   void render(float percent,SDL_Color foreground);

private:
   int x;
   int y;
   int w;
   int h;
   SDL_Color foreground;
   SDL_Color background;
   SDL_Renderer* _gRenderer;


};