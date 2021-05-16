#pragma once

#include <SDL2/SDL.h>

#include <iostream>

#include "Textures/LTexture.hpp"
#include "UI/ProgressBar.h"
#include "utils.hpp"

using namespace std;

class BottomBar {
 public:
  BottomBar();
  void init(int x, int y, int w, int h);
  void setRenderer(SDL_Renderer* _gRenderer);

  void render();
  void initViews();
  void update(int coins, int activePoints, string weapon_stats);

 private:
  int x;
  int y;
  int h;
  int w;
  SDL_Renderer* _gRenderer;
  TTF_Font* lazy_font_60;

  LTexture CoinImage;
  LTexture CoinsCollectedText;
  LTexture weaponText;

  int coinsCollected = 0;
  int prevCoinsCollected = 0;
  int activePoints = 0;
  string weapon_stats = "-";

  ProgressBar pacmanPointBar;
};