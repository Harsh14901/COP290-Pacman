#pragma once


#include <iostream>
#include <map>
using namespace std;



class PreferenceManager{
public:
    static void initSettings();
    static bool MUSIC_ON;
    static bool SFX_ON;
    static string THEME;
    static int NUM_ENEMIES;
};