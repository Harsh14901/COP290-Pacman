#pragma once

#include <SDL2/SDL.h>

#include "bits/stdc++.h"
using namespace std;

vector<int> extractIntegerWords(string str);
SDL_Color HSVtoRGB(float H, float S, float V);
unordered_map<string, string> convert_string_to_map(string s);
string map_to_string(unordered_map<string, string> m);
