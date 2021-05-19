#include "Utils/PreferenceManager.hpp"

bool PreferenceManager::MUSIC_ON;
bool PreferenceManager::SFX_ON;
Themes PreferenceManager::THEME;
int PreferenceManager::NUM_ENEMIES;

void PreferenceManager::initSettings(){
    MUSIC_ON = true;
    SFX_ON = true;
    THEME = Themes::AVENGERS;
    NUM_ENEMIES = 2;
}