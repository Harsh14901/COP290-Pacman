#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <SDL2/SDL.h>
#include <map>
using namespace std;

vector<int> extractIntegerWords(string str);
SDL_Color HSVtoRGB(float H, float S,float V);
map<string, string> convert_string_to_map(string s);
string map_to_string(map<string,string>  m);

