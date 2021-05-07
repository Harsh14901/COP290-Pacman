#include "UI/ProgressBar.h"

ProgressBar::ProgressBar(){}
ProgressBar::ProgressBar(int x,int y,int w, int h,SDL_Color foreground,SDL_Color background){
    init(x,y,w,h,foreground,background);
}

void ProgressBar::init(int x,int y,int w, int h,SDL_Color foreground,SDL_Color background){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->foreground = foreground;
    this->background = background;
}
void ProgressBar::setRenderer( SDL_Renderer* _gRenderer){
    this->_gRenderer = _gRenderer;
}

void ProgressBar::render(float percent) {
    render(percent,foreground);
}


void ProgressBar::render(float percent,SDL_Color foreground) {
   percent = percent > 1.f ? 1.f : percent < 0.f ? 0.f : percent;
   SDL_Color old;
   SDL_GetRenderDrawColor(_gRenderer, &old.r, &old.g, &old.g, &old.a);
   SDL_Rect bgrect = { x, y, w, h };
   SDL_SetRenderDrawColor(_gRenderer, background.r, background.g, background.b, background.a);
   SDL_RenderFillRect(_gRenderer, &bgrect);
   SDL_SetRenderDrawColor(_gRenderer, foreground.r, foreground.g, foreground.b, foreground.a);
   int pw = (int)((float)w * percent);
   int px = x;// + (w - pw);
   SDL_Rect fgrect = { px, y, pw, h };
   SDL_RenderFillRect(_gRenderer, &fgrect);
   SDL_SetRenderDrawColor(_gRenderer, old.r, old.g, old.b, old.a);
}