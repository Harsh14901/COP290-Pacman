#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include "Textures/LTexture.hpp"
#include "UI/ImageButton.hpp"
#include "PreferenceManager.hpp"
#include "Utils/CommonAudios.hpp"
using namespace std;

class SettingsScreen
{

public:
    SettingsScreen();
    SettingsScreen(int x, int y, int w, int h);
    void init(int x, int y, int w, int h); // Defaults to clicked
    void render(float percent);
    void setRenderer(SDL_Renderer *_gRenderer);
    void render();
    void toggle_click();
    void set_clicked(bool clicked);
    void handleEvent(SDL_Event &evnt);

private:
    int x;
    int y;
    int w;
    int h;
    SDL_Renderer *_gRenderer;

    const static int themeOptions = 9;
    ImageButton themeButtons[themeOptions];
    ImageButton musicButton;
    ImageButton sfxButton;
    LTexture musicText;
    LTexture sfxText;
    LTexture musicTextSelected;
    LTexture sfxTextSelected;
    LTexture numEnemiesText[4];
    LTexture numEnemiesSelectedText[4];

    LTexture themeSectionText;
    LTexture musicSelectionText;
    LTexture gamePlaySelectionText;

    int selectedTheme = 0;
    int sectionCursor = 0;
    int gamePlayOption = 0;
    int num_sections = 3;

    int current_music_option_selected = 0;

    void handleThemeEvent(int keycode);
    void handleSoundEvent(int keycode);
    void handleGamePlayEvent(int keycode);

    SDL_Texture* loadTexture(string path);
    SDL_Texture* _backgroundTexture = NULL;

    void selectInitialTheme();
    void updateTheme();
    void initThemeButtons(SDL_Renderer* _gRenderer);


};