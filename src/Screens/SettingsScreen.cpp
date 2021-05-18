#include "Screens/SettingsScreen.hpp"

SettingsScreen::SettingsScreen(){}
SettingsScreen::SettingsScreen(int x,int y,int w, int h){
    init(x,y,w,h);
}

void SettingsScreen::init(int x,int y,int w, int h){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

void SettingsScreen::setRenderer(SDL_Renderer* _gRenderer){
    this->_gRenderer = _gRenderer;


    // First Section: Width/3, Height Full
    int offX = 0.06*w + 0;
    int offY = h/8.0  + 0;
    themeButtons[0].init(offX + 0.03 * w,offY + 0.10 * h,
                     0.16 * w, 0.07 * h,
                     "assets/buttons/settings/pacman.png",
                     "assets/buttons/settings/pacman_out.png");
    themeButtons[1].init(offX + 0.03 * w,offY + 0.20 * h,
                     0.16 * w, 0.07 * h,
                     "assets/buttons/settings/avengers.png",
                     "assets/buttons/settings/avengers_out.png");
    themeButtons[2].init(offX + 0.03 * w,offY + 0.30 * h,
                     0.16 * w, 0.07 * h,
                     "assets/buttons/settings/minecraft.png",
                     "assets/buttons/settings/minecraft_out.png");
    themeButtons[3].init(offX + 0.03 * w,offY + 0.40 * h,
                     0.16 * w, 0.07 * h,
                     "assets/buttons/settings/covid.png",
                     "assets/buttons/settings/covid_out.png");
    for(int i=0;i<themeOptions;i++){
        themeButtons[i].setRenderer(_gRenderer);
    }

}

void SettingsScreen::handleEvent(SDL_Event &evnt){
    return;
}




void SettingsScreen::render() {

    for(int i=0;i<themeOptions;i++){
        themeButtons[i].set_clicked(selectedTheme==i);
        themeButtons[i].render();
    }

}