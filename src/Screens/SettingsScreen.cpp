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
    sfxText.setRenderer(_gRenderer);
    musicText.setRenderer(_gRenderer);

    offX = (0.34 +0.06)*w;
    offY = h/8.0  + 0;

    sfxText.loadFromRenderedText(
      "SFX", {255, 255, 255}, TTF_OpenFont("assets/fonts/win_font.ttf", 60));
    musicText.loadFromRenderedText(
      "MUSIC", {255, 255, 255}, TTF_OpenFont("assets/fonts/win_font.ttf", 60));

    sfxButton.init(  offX+0.16*w,offY + 0.21*h,0.12*w,0.08*h,"assets/buttons/settings/switch_on.png","assets/buttons/settings/switch_off.png");
    musicButton.init(offX+0.16*w,offY + 0.11*h,0.12*w,0.08*h,"assets/buttons/settings/switch_on.png","assets/buttons/settings/switch_off.png");

    sfxButton.setRenderer(_gRenderer);
    musicButton.setRenderer(_gRenderer);

}

void SettingsScreen::handleThemeEvent(int keycode){
        switch (keycode) {
            case SDLK_DOWN:
                selectedTheme = (selectedTheme+1)%themeOptions;
                break;
            case SDLK_UP:
                selectedTheme = (selectedTheme-1+themeOptions*1000)%themeOptions;
                break;
            default:
                cout << "Invalid Key, Play Sound" << endl;
        }
}

void SettingsScreen::handleSoundEvent(int keycode){
        switch (keycode) {
            case SDLK_DOWN:
                current_music_option_selected = (current_music_option_selected+1)%2;
                break;
            case SDLK_UP:
                current_music_option_selected = (current_music_option_selected-1+2*1000)%2;
                break;
            case 13: // Enter
                if(current_music_option_selected==0) musicToggle = !musicToggle;
                if(current_music_option_selected==1) sfxToggle = !sfxToggle;
                break;
            default:
                cout << "Invalid Key, Play Sound: " << keycode  << endl;
        }
}

void SettingsScreen::handleEvent(SDL_Event &evnt){
    switch (evnt.type) {
        case SDL_KEYDOWN:
          auto key = evnt.key.keysym.sym;
          if(key==9) sectionCursor = (sectionCursor+1)%num_sections; // TAB KEY
          cout << "Selection Cursor " << sectionCursor << endl;
          cout << "Key Pressssssed " << key << endl;
          if(sectionCursor==0) handleThemeEvent(key);
          if(sectionCursor==1) handleSoundEvent(key);
    }
}




void SettingsScreen::render() {


    for(int i=0;i<themeOptions;i++){
        themeButtons[i].set_clicked(selectedTheme==i);
        themeButtons[i].render();
    }

    musicText.render(0.43*w,0.11*h+h/8.0);
    sfxText.render(0.43*w,0.21*h+h/8.0);

    musicButton.set_clicked(musicToggle);
    sfxButton.set_clicked(sfxToggle);
    musicButton.render();
    sfxButton.render();


}