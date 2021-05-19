#include "CommonAudios.hpp"


AudioAsset CommonAudios::buttonHover;
AudioAsset CommonAudios::buttonClick;
AudioAsset CommonAudios::buttonStart;

void CommonAudios::initialize_sound(){
    buttonHover.init("assets/sounds/button_hover.ogg",false);
    buttonClick.init("assets/sounds/button_click.ogg",false);
    buttonStart.init("assets/sounds/button_start.ogg",false);
}