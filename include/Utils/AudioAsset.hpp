#pragma once

#include <SDL2/SDL_mixer.h>
#include <iostream>

using namespace std;

class AudioAsset{

public:
    void init(string sound,bool isMusic);
    // AudioAsset(string sound,bool isMusic);
    string sound_name;
    bool is_music;
    bool is_playing;
    bool is_loop;
    void play();
    void pause();
    void reset();
    void stop();
private:
    Mix_Chunk *soundEffect = NULL;
    Mix_Music *music = NULL;

    
};