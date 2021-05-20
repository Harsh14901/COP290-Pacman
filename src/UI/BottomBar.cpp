#include "UI/BottomBar.hpp"

BottomBar::BottomBar() {}

void BottomBar::init(int x, int y, int w, int h) {
  this->x = x;
  this->y = y;
  this->w = w;
  this->h = h;
  lazy_font_60 = TTF_OpenFont("assets/fonts/Rajdhani.ttf", 60);
  if (lazy_font_60 == nullptr) {
    fatalError(TTF_GetError());
  }
}

void BottomBar::initViews() {
  CoinImage.setRenderer(_gRenderer);
  CoinImage.loadFromFile("assets/pngs/coin3.png");
  CoinImage.set_image_dimenstions(0.9 * (h / 2), h / 2);

  CoinsCollectedText.setRenderer(_gRenderer);
  CoinsCollectedText.loadFromRenderedText("0", {210, 255, 30}, lazy_font_60);

  weaponText.setRenderer(_gRenderer);
  weaponText.loadFromRenderedText(weapon_stats, {210, 255, 30}, lazy_font_60);

  pacmanPointBar.init(x + 40 + 0.9 * (h / 2) + 110, y + h / 4 + 10, w / 4, h / 3,
                      SDL_Color{100, 255, 40, 255},
                      SDL_Color{255, 255, 255, 200});
  pacmanPointBar.setRenderer(_gRenderer);
}

void BottomBar::update(int coins, int activePoints, string weapon_stats) {
  this->coinsCollected = coins;
  this->activePoints = activePoints;
  this->weapon_stats = weapon_stats;
}

void BottomBar::setRenderer(SDL_Renderer* _gRenderer) {
  this->_gRenderer = _gRenderer;
}

void BottomBar::render() {
  CoinImage.render(x + 30, y + h / 6 + 10);
  if (coinsCollected != prevCoinsCollected) {
    CoinsCollectedText.loadFromRenderedText(to_string(coinsCollected),
                                            {210, 255, 30}, lazy_font_60);
  }

  CoinsCollectedText.render(x + 30 + 0.9 * (h / 2) + 10, y + h / 6);

  weaponText.loadFromRenderedText(weapon_stats, {210, 255, 30}, lazy_font_60);
  weaponText.render(x + 30 + 10 * (h / 2) + 10, y + h / 6);

  pacmanPointBar.render(activePoints / 100.0,
                        HSVtoRGB(240 - 1.2 * activePoints, 100, 100));

  prevCoinsCollected = coinsCollected;
}
