#include "UI/FloatingText.hpp"


FloatingText::FloatingText(string text,int mPosX,int mPosY){
    this->text = text;
    this->x = mPosX;
    this->y = mPosY;
    this->origY = y;
}

void FloatingText::init(SDL_Renderer* _gRenderer){
    this->_gRenderer = _gRenderer;
    texture.setRenderer(this->_gRenderer);
    texture.loadFromRenderedText(text,{20,255,20},TTF_OpenFont("assets/fonts/win_font.ttf", 40));
}

void FloatingText::setPos(string text,int x,int y,bool pos){
    this->text = text;
    if(pos) texture.loadFromRenderedText(text,{20,255,20},TTF_OpenFont("assets/fonts/win_font.ttf", 40));
    else texture.loadFromRenderedText(text,{255,20,20},TTF_OpenFont("assets/fonts/win_font.ttf", 40));
    this->x = x;
    this->y = y;
    this->origY = y;
    disp = true;
}

void FloatingText::render(){
    if(!disp) return;
    texture.render(x,y);
    y -= 1;
    if(y-origY<-100) disp = false;
}