#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include "Textures/LTexture.hpp"

using namespace std;


class ImageButton{

public:
   ImageButton();
   ImageButton(int x,int y,int w, int h,string asset_clicked,string asset_unclicked);
   void init(int x,int y,int w, int h,string asset_clicked,string asset_unclicked); // Defaults to clicked
   void render(float percent);
   void setRenderer( SDL_Renderer* _gRenderer);
   void render();
   void toggle_click();
   void set_clicked(bool clicked);

private:
   int x;
   int y;
   int w;
   int h;
   bool is_clicked = false;
   string asset_clicked;
   string asset_unclicked;
   SDL_Renderer* _gRenderer;

   LTexture texture_selected;
   LTexture texture_unselected;



};