#pragma once

#include "Utils/AudioAsset.hpp"
using namespace std;

class CommonAudios{
public:
    static AudioAsset buttonHover;
    static AudioAsset buttonClick;
    static AudioAsset buttonStart;
    static void initialize_sound();

};