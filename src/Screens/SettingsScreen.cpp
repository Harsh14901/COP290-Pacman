#include "Screens/SettingsScreen.hpp"

#include "Constants.hpp"

extern bool is_server;
SettingsScreen::SettingsScreen() {}
SettingsScreen::SettingsScreen(int x, int y, int w, int h) { init(x, y, w, h); }

void SettingsScreen::init(int x, int y, int w, int h) {
  this->x = x;
  this->y = y;
  this->w = w;
  this->h = h;
}

SDL_Texture* SettingsScreen::loadTexture(string path) {
  SDL_Texture* newTexture = NULL;
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  newTexture = SDL_CreateTextureFromSurface(_gRenderer, loadedSurface);
  SDL_FreeSurface(loadedSurface);
  return newTexture;
}

void SettingsScreen::selectInitialTheme() {
  auto theme = PreferenceManager::THEME;
  selectedTheme = int(theme) - 1;
}

void SettingsScreen::updateTheme() {
  PreferenceManager::THEME = Themes(selectedTheme + 1);
}

void SettingsScreen::initThemeButtons(SDL_Renderer* _gRenderer) {
  int offX = 0.06 * w + 0;
  int offY = h / 7.0 + 0;

  string assets[] = {"pacman", "avengers",  "minecraft",    "covid", "jungle",
                     "aliens", "star_wars", "transformers", "mario"};

  for (int i = 0; i < themeOptions; i++) {
    themeButtons[i].init(offX + 0.03 * w, offY + (i * 0.07 + 0.1) * h, 0.16 * w,
                         0.06 * h,
                         "assets/buttons/settings/" + assets[i] + ".png",
                         "assets/buttons/settings/" + assets[i] + "_out.png");
    themeButtons[i].setRenderer(_gRenderer);
    cout << "YO " << i << endl;
  }
}

void SettingsScreen::initWeaponsButtons(SDL_Renderer* _gRenderer) {
  int offX = 0.7 * w + 0;
  int offY = h / 7.0 + 0.18 * h;

  string assets[] = {"freeze_gun", "emp", "wall_buster", "bomb"};

  for (int i = 0; i < 4; i++) {
    weaponsButton[i].init(offX, offY + (i * 0.12 + 0.10) * h, 0.24 * w,
                          0.09 * h,
                          "assets/buttons/settings/" + assets[i] + ".png",
                          "assets/buttons/settings/" + assets[i] + "_out.png");
    weaponsButton[i].setRenderer(_gRenderer);
  }
}

void SettingsScreen::setRenderer(SDL_Renderer* _gRenderer) {
  this->_gRenderer = _gRenderer;

  selectInitialTheme();

  _backgroundTexture =
      loadTexture("assets/backgrounds/settings_background2.png");
  SDL_SetTextureAlphaMod(_backgroundTexture, 100);
  cout << "Height an W are:" << h << " " << w << endl;

  // First Section: Width/3, Height Full
  int offX = 0.06 * w + 0;
  int offY = h / 7.0 + 0;
  // themeButtons[0].init(offX + 0.03 * w,offY + 0.10 * h,
  //                  0.16 * w, 0.07 * h,
  //                  "assets/buttons/settings/pacman.png",
  //                  "assets/buttons/settings/pacman_out.png");
  // themeButtons[1].init(offX + 0.03 * w,offY + 0.20 * h,
  //                  0.16 * w, 0.07 * h,
  //                  "assets/buttons/settings/avengers.png",
  //                  "assets/buttons/settings/avengers_out.png");
  // themeButtons[2].init(offX + 0.03 * w,offY + 0.30 * h,
  //                  0.16 * w, 0.07 * h,
  //                  "assets/buttons/settings/minecraft.png",
  //                  "assets/buttons/settings/minecraft_out.png");
  // themeButtons[3].init(offX + 0.03 * w,offY + 0.40 * h,
  //                  0.16 * w, 0.07 * h,
  //                  "assets/buttons/settings/covid.png",
  //                  "assets/buttons/settings/covid_out.png");
  initThemeButtons(_gRenderer);
  initWeaponsButtons(_gRenderer);

  auto xx = PreferenceManager::playerBullets;
  weapon_selected_prev = int(xx.first);
  weapon_selected_now = int(xx.second);

  themeSectionText.setRenderer(_gRenderer);
  musicSelectionText.setRenderer(_gRenderer);
  gamePlaySelectionText.setRenderer(_gRenderer);

  sfxText.setRenderer(_gRenderer);
  musicText.setRenderer(_gRenderer);
  sfxTextSelected.setRenderer(_gRenderer);
  musicTextSelected.setRenderer(_gRenderer);

  cout << "Line 94" << endl;

  offX = (0.28 + 0.06) * w;
  offY = h / 7.0 + 0;

  musicSelectionText.loadFromRenderedText(
      "SOUNDS", {255, 255, 255}, TTF_OpenFont("assets/fonts/win_font.ttf", 80));

  themeSectionText.loadFromRenderedText(
      "THEMES", {255, 255, 255}, TTF_OpenFont("assets/fonts/win_font.ttf", 80));

  gamePlaySelectionText.loadFromRenderedText(
      "GAMEPLAY", {255, 255, 255},
      TTF_OpenFont("assets/fonts/win_font.ttf", 80));

  sfxText.loadFromRenderedText("SFX", {255, 255, 255},
                               TTF_OpenFont("assets/fonts/Rajdhani.ttf", 60));
  musicText.loadFromRenderedText("MUSIC", {255, 255, 255},
                                 TTF_OpenFont("assets/fonts/Rajdhani.ttf", 60));
  for (int i = 0; i < 4; i++) {
    numEnemiesSelectedText[i].setRenderer(_gRenderer);
    numEnemiesText[i].setRenderer(_gRenderer);

    numEnemiesText[i].loadFromRenderedText(
        "Enemies: " + to_string(i + 1), {255, 255, 255},
        TTF_OpenFont("assets/fonts/Rajdhani.ttf", 60));
    numEnemiesSelectedText[i].loadFromRenderedText(
        "Enemies: " + to_string(i + 1), {210, 255, 30},
        TTF_OpenFont("assets/fonts/Rajdhani.ttf", 60));
  }

  chooseWeaponsText.setRenderer(_gRenderer);
  chooseWeaponsText.loadFromRenderedText(
      "PLAYER WEAPONS", {255, 255, 255},
      TTF_OpenFont("assets/fonts/Rajdhani.ttf", 50));

  sfxTextSelected.loadFromRenderedText(
      "SFX", {210, 255, 30}, TTF_OpenFont("assets/fonts/Rajdhani.ttf", 60));
  musicTextSelected.loadFromRenderedText(
      "MUSIC", {210, 255, 30}, TTF_OpenFont("assets/fonts/Rajdhani.ttf", 60));

  sfxButton.init(offX + 0.16 * w, offY + 0.09 * h, 0.12 * w, 0.08 * h,
                 "assets/buttons/settings/switch_modern_on.png",
                 "assets/buttons/settings/switch_modern_off.png");
  musicButton.init(offX + 0.16 * w, offY + 0.19 * h, 0.12 * w, 0.08 * h,
                   "assets/buttons/settings/switch_modern_on.png",
                   "assets/buttons/settings/switch_modern_off.png");

  sfxButton.setRenderer(_gRenderer);
  musicButton.setRenderer(_gRenderer);

  cout << "Alright Init Done" << endl;
}

void SettingsScreen::handleThemeEvent(int keycode) {
  switch (keycode) {
    case SDLK_DOWN:
      selectedTheme = (selectedTheme + 1) % themeOptions;
      CommonAudios::buttonHover.play();
      break;
    case SDLK_UP:
      CommonAudios::buttonHover.play();
      selectedTheme = (selectedTheme - 1 + themeOptions * 1000) % themeOptions;
      break;
    default:
      cout << "Invalid Key, Play Sound" << endl;
  }
  updateTheme();
}

void SettingsScreen::handleSoundEvent(int keycode) {
  switch (keycode) {
    case SDLK_DOWN:
      CommonAudios::buttonHover.play();
      current_music_option_selected = (current_music_option_selected + 1) % 2;
      break;
    case SDLK_UP:
      CommonAudios::buttonHover.play();
      current_music_option_selected =
          (current_music_option_selected - 1 + 2 * 1000) % 2;
      break;
    case 13:  // Enter
      if (current_music_option_selected == 0)
        PreferenceManager::MUSIC_ON = !PreferenceManager::MUSIC_ON;
      if (current_music_option_selected == 1)
        PreferenceManager::SFX_ON = !PreferenceManager::SFX_ON;
      CommonAudios::buttonClick.play();
      break;
    default:
      cout << "Invalid Key, Play Sound: " << keycode << endl;
  }
}

void SettingsScreen::handleGamePlayOption(bool inc) {
  if (gamePlayOption > 0) {
    if (weapon_entered_press) {
      // handle prev_weapon
      if (gamePlayOption - 1 == weapon_selected_now) {
        gamePlayOption = 1 + ((gamePlayOption - 1) + inc + 4) % 4;
      }
      weapon_selected_prev = gamePlayOption - 1;
    } else {
      if (gamePlayOption - 1 == weapon_selected_prev) {
        gamePlayOption = 1 + ((gamePlayOption - 1) + inc + 4) % 4;
      }
      weapon_selected_now = gamePlayOption - 1;
    }
  }
  if (is_server) {
    PreferenceManager::playerBullets = make_pair(
        BulletType(weapon_selected_prev), BulletType(weapon_selected_now));
  } else {
    PreferenceManager::enemyBullets = make_pair(
        BulletType(weapon_selected_prev), BulletType(weapon_selected_now));
  }
}

void SettingsScreen::handleGamePlayEvent(int keycode) {
  switch (keycode) {
    case SDLK_DOWN:
      gamePlayOption = (gamePlayOption + 1) % 5;
      cout << "Game Play Option is: " << gamePlayOption << endl;
      handleGamePlayOption(true);
      CommonAudios::buttonHover.play();
      break;
    case SDLK_UP:
      gamePlayOption = (gamePlayOption - 1 + 1 * 1000) % 5;
      handleGamePlayOption(false);
      CommonAudios::buttonHover.play();
      break;
    case SDLK_RIGHT:
      PreferenceManager::NUM_ENEMIES =
          min(PreferenceManager::NUM_ENEMIES + 1, 4);
      CommonAudios::buttonClick.play();
      break;
    case SDLK_LEFT:
      PreferenceManager::NUM_ENEMIES =
          max(PreferenceManager::NUM_ENEMIES - 1, 1);
      CommonAudios::buttonClick.play();
      break;
    case 13:
      weapon_entered_press = !weapon_entered_press;
    default:
      cout << "Invalid Key, Play Sound: " << keycode << endl;
  }
}

void SettingsScreen::handleEvent(SDL_Event& evnt) {
  switch (evnt.type) {
    case SDL_KEYDOWN:
      auto key = evnt.key.keysym.sym;
      if (key == 9)
        sectionCursor = (sectionCursor + 1) % num_sections;  // TAB KEY
      CommonAudios::buttonHover.play();
      cout << "Selection Cursor " << sectionCursor << endl;
      cout << "Key Presssed " << key << endl;
      if (sectionCursor == 0) handleThemeEvent(key);
      if (sectionCursor == 1) handleSoundEvent(key);
      if (sectionCursor == 2) handleGamePlayEvent(key);
  }
}

void SettingsScreen::render() {
  SDL_RenderCopy(_gRenderer, _backgroundTexture, NULL, NULL);

  for (int i = 0; i < themeOptions; i++) {
    themeButtons[i].set_clicked(selectedTheme == i);
    themeButtons[i].render();
  }

  for (int i = 0; i < 4; i++) {
    weaponsButton[i].set_clicked(weapon_selected_prev == i ||
                                 weapon_selected_now == i);
    weaponsButton[i].render();
  }

  if (sectionCursor == 1) {
    if (current_music_option_selected == 0) {
      musicTextSelected.render(0.37 * w, 0.19 * h + h / 7.0);
      sfxText.render(0.37 * w, 0.09 * h + h / 7.0);
    } else {
      musicText.render(0.37 * w, 0.19 * h + h / 7.0);
      sfxTextSelected.render(0.37 * w, 0.09 * h + h / 7.0);
    }
  } else {
    musicText.render(0.37 * w, 0.19 * h + h / 7.0);
    sfxText.render(0.37 * w, 0.09 * h + h / 7.0);
  }
  if (sectionCursor == 2 && gamePlayOption == 0) {
    numEnemiesSelectedText[PreferenceManager::NUM_ENEMIES - 1].render(
        0.718 * w, 0.09 * h + h / 7.0);
  } else {
    numEnemiesText[PreferenceManager::NUM_ENEMIES - 1].render(
        0.718 * w, 0.09 * h + h / 7.0);
  }
  chooseWeaponsText.render(0.678 * w, 0.19 * h + h / 7.0);

  themeSectionText.render(0.085 * w, 0.12 * h);
  musicSelectionText.render(0.405 * w, 0.12 * h);
  gamePlaySelectionText.render(0.705 * w, 0.12 * h);

  musicButton.set_clicked(PreferenceManager::MUSIC_ON);
  sfxButton.set_clicked(PreferenceManager::SFX_ON);
  musicButton.render();
  sfxButton.render();
}