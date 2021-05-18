#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include "Textures/LTexture.hpp"
#include "UI/ImageButton.hpp"
#include "PreferenceManager.hpp"
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

    const static int themeOptions = 4;
    ImageButton themeButtons[themeOptions];
    int selectedTheme = 0;
};