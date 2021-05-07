#include "Utils/AudioAsset.hpp"

extern PreferenceManager prefManager;

void AudioAsset::init(string sound,bool isMusic){
    sound_name = sound;
    is_music = isMusic;
    if(is_music){
        cout << "Is it music " << endl;
        music = Mix_LoadMUS(&sound[0]);
        is_loop = true;
    }else{    
        soundEffect = Mix_LoadWAV(&sound[0]);
        if(soundEffect==NULL){
            cerr << "Invalid Filename" << endl;
        }
        // cout << "File Loaded Success" << endl;
    }
    applySettings();
}

void AudioAsset::applySettings(){
    Mix_Volume(-1,0);
    if(soundEffect != NULL){
        cout << "Come On I set the volume to zero" << endl;
        if(prefManager.getSettingValue("sfx")=="false") Mix_VolumeChunk(soundEffect,0);
    }if(music!=NULL){
        cout << "Come On I set the music volume to zero" << endl;
        if(prefManager.getSettingValue("music")=="false") Mix_VolumeMusic(0);
    }
}

void AudioAsset::play(){
    // if(is_playing){
    //     // TODO: Decide If error should be thrown or music should be played again or ignored or queued
    //     // stop();
    //     // play();
    //     return;
    // }
    int loops = is_loop?-1:0;
    is_playing = true;
    if(is_music)   
    {
        // cout << "Music Should be playing" << endl;
        Mix_PlayMusic(music,-1);
    }
    else
    {
        // cout << "Should Play Now" << endl;
        Mix_PlayChannel(-1,soundEffect,0);
    } 


}

void AudioAsset::pause(){
    if( Mix_PausedMusic() != 1 )
    {
        //Resume the music
        Mix_ResumeMusic();
    }
    is_playing = false;
}

void AudioAsset::stop(){
    Mix_HaltMusic();
}


