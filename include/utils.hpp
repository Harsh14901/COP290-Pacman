#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <SDL2/SDL.h>
using namespace std;

vector<int> extractIntegerWords(string str);
SDL_Color HSVtoRGB(float H, float S,float V);
