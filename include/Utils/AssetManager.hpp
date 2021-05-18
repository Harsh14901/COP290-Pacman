#pragma once

#include <iostream>
#include "map"
#include "vector"
#include <bits/stdc++.h>

using namespace std;

enum Themes {
    MASTER,
    PACMAN,
    AVENGERS,
    T_Last  // Placeholder
};

enum ThemeAssets{
    FOLDER_NAME,  // should not be accessed by outside
    PACMAN_SPRITE,
    ENEMY_SPRITE,
    VENT_PNG,  // Note corresponding vent open and close are calculated automatically
    COIN_PNG,
    COIN_SOUND,
    CHERRY_PNG,
    FREEZEBULLET_PNG,
    FREEZEBULLET_SOUND,
    E_Last
};


// The main purpose of this class is to get back asset string from theme and key
class AssetManager{
public:

    AssetManager();

    void set_theme(Themes theme);
    Themes get_theme();

    string get_asset(ThemeAssets key);

    void init(Themes startTheme);


private:
    Themes selected_theme;
    map<Themes,map<ThemeAssets,string>> data;
    string getFolderNameAppended(string fol,string val);



};