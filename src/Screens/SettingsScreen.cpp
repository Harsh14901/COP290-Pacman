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

SDL_Texture* SettingsScreen::loadTexture(string path){
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    newTexture = SDL_CreateTextureFromSurface(_gRenderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

void SettingsScreen::setRenderer(SDL_Renderer* _gRenderer){
    this->_gRenderer = _gRenderer;

    _backgroundTexture = loadTexture("assets/backgrounds/settings_background2.png");
    SDL_SetTextureAlphaMod(_backgroundTexture, 100);
    cout << "Height an W are:" << h << " " << w << endl;

    // First Section: Width/3, Height Full
    int offX = 0.06*w + 0;
    int offY = h/7.0  + 0;
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

    themeSectionText.setRenderer(_gRenderer);
    musicSelectionText.setRenderer(_gRenderer);

    sfxText.setRenderer(_gRenderer);
    musicText.setRenderer(_gRenderer);
    sfxTextSelected.setRenderer(_gRenderer);
    musicTextSelected.setRenderer(_gRenderer);


    offX = (0.28 +0.06)*w;
    offY = h/7.0  + 0;

    musicSelectionText.loadFromRenderedText(
      "SOUNDS", {255, 255, 255}, TTF_OpenFont("assets/fonts/win_font.ttf", 80));

    themeSectionText.loadFromRenderedText(
      "THEMES", {255, 255, 255}, TTF_OpenFont("assets/fonts/win_font.ttf", 80));

    sfxText.loadFromRenderedText(
      "SFX", {255, 255, 255}, TTF_OpenFont("assets/fonts/Rajdhani.ttf", 60));
    musicText.loadFromRenderedText(
      "MUSIC", {255, 255, 255}, TTF_OpenFont("assets/fonts/Rajdhani.ttf", 60));

    sfxTextSelected.loadFromRenderedText(
      "SFX", {210, 255, 30}, TTF_OpenFont("assets/fonts/Rajdhani.ttf", 60));
    musicTextSelected.loadFromRenderedText(
      "MUSIC", {210, 255, 30}, TTF_OpenFont("assets/fonts/Rajdhani.ttf", 60));


    sfxButton.init(  offX+0.16*w,offY + 0.09*h,0.12*w,0.08*h,"assets/buttons/settings/switch_modern_on.png","assets/buttons/settings/switch_modern_off.png");
    musicButton.init(offX+0.16*w,offY + 0.19*h,0.12*w,0.08*h,"assets/buttons/settings/switch_modern_on.png","assets/buttons/settings/switch_modern_off.png");

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

    SDL_RenderCopy(_gRenderer, _backgroundTexture, NULL, NULL);


    for(int i=0;i<themeOptions;i++){
        themeButtons[i].set_clicked(selectedTheme==i);
        themeButtons[i].render();
    }

    if(sectionCursor==1){
        if(current_music_option_selected==0){
            musicText.render(0.37*w,0.09*h+h/7.0);
            sfxTextSelected.render(0.37*w,0.19*h+h/7.0);
        }else{
            musicTextSelected.render(0.37*w,0.09*h+h/7.0);
            sfxText.render(0.37*w,0.19*h+h/7.0);
        }
    }else{
        musicText.render(0.37*w,0.09*h+h/7.0);
        sfxText.render(0.37*w,0.19*h+h/7.0);
    }

    themeSectionText.render(0.085*w,0.12*h);
    musicSelectionText.render(0.405*w,0.12*h);


    musicButton.set_clicked(musicToggle);
    sfxButton.set_clicked(sfxToggle);
    musicButton.render();
    sfxButton.render();


}