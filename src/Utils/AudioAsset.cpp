#include "Utils/AudioAsset.hpp"


AudioAsset::AudioAsset() {}

AudioAsset::AudioAsset(string sound, bool isMusic) : AudioAsset() {
  init(sound, isMusic);
}

void AudioAsset::init(string sound, bool isMusic) {
  sound_name = sound;
  is_music = isMusic;
  if (is_music) {
    cout << "Is it music " << endl;
    music = Mix_LoadMUS(&sound[0]);
    is_loop = true;
  } else {
    soundEffect = Mix_LoadWAV(sound.c_str());
    cout << "loading sound asset " << sound.c_str() << endl;
    if (soundEffect == nullptr) {
      cerr << "Invalid Filename: " << sound << endl;
    }
    // cout << "File Loaded Success" << endl;
  }
  applySettings();
}

void AudioAsset::applySettings() {
  // Mix_Volume(-1,0);
  if (soundEffect != NULL) {
    if (!PreferenceManager::SFX_ON) Mix_VolumeChunk(soundEffect, 0);
    else Mix_VolumeChunk(soundEffect,MIX_MAX_VOLUME);
  }
  if (music != NULL) {
    if (!PreferenceManager::MUSIC_ON) Mix_VolumeMusic(0);
    else Mix_VolumeMusic(MIX_MAX_VOLUME);
  }
}

void AudioAsset::play() {
  // if(is_playing){
  //     // TODO: Decide If error should be thrown or music should be played
  //     again or ignored or queued
  //     // stop();
  //     // play();
  //     return;
  // }
  cout << "Should be playing chunk "  << sound_name << endl;
  // int loops = is_loop ? -1 : 0;
  is_playing = true;
  if (is_music) {
    cout << "Music Should be playing" << endl;
    Mix_PlayMusic(music, -1);
  } else {
    // cout << "Should Play Now" << endl;
    Mix_PlayChannel(-1, soundEffect, 0);
  }
}

void AudioAsset::pause() {
  if (Mix_PausedMusic() != 1) {
    // Resume the music
    Mix_ResumeMusic();
  }
  is_playing = false;
}

void AudioAsset::stop() { Mix_HaltMusic(); }
