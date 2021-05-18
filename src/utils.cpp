#include "utils.hpp"

#include <regex>


vector<int> extractIntegerWords(string str) {
  vector<int> numbers;
  int num = 0;

  for (int i = 0; i < (int)str.length(); i++) {
    if (isdigit(str[i])) {
      num = (num * 10) + (str[i] - '0');
    } else {
      if (num > 0) {
        numbers.push_back(num);
        num = 0;
      }
    }
  }

  // Save the last number if not zero
  if (num > 0) {
    numbers.push_back(num);
  }
  return numbers;
}

unordered_map<string, string> convert_string_to_map(string s) {
  unordered_map<string, string> m;

  string::size_type key_pos = 0;
  string::size_type key_end;
  string::size_type val_pos;
  string::size_type val_end;

  while ((key_end = s.find(':', key_pos)) != string::npos) {
    if ((val_pos = s.find_first_not_of(": ", key_end)) == string::npos) break;

    val_end = s.find('\n', val_pos);
    m.emplace(s.substr(key_pos, key_end - key_pos),
              s.substr(val_pos, val_end - val_pos));

    key_pos = val_end;
    if (key_pos != string::npos) ++key_pos;
  }

  return m;
}

string map_to_string(unordered_map<string, string> m) {
  string output = "";
  string convrt = "";
  string result = "";

  for (auto it = m.cbegin(); it != m.cend(); it++) {
    convrt = it->second;
    output += (it->first) + ":" + (convrt) + "\n";
  }

  result = output.substr(0, output.size() - 1);

  return result;
}

SDL_Color HSVtoRGB(float H, float S, float V) {
  if (H > 360 || H < 0 || S > 100 || S < 0 || V > 100 || V < 0) {
    cout << "The givem HSV values are not in valid range" << endl;
    // return;
  }
  float s = S / 100;
  float v = V / 100;
  float C = s * v;
  float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
  float m = v - C;
  float r, g, b;
  if (H >= 0 && H < 60) {
    r = C, g = X, b = 0;
  } else if (H >= 60 && H < 120) {
    r = X, g = C, b = 0;
  } else if (H >= 120 && H < 180) {
    r = 0, g = C, b = X;
  } else if (H >= 180 && H < 240) {
    r = 0, g = X, b = C;
  } else if (H >= 240 && H < 300) {
    r = X, g = 0, b = C;
  } else {
    r = C, g = 0, b = X;
  }
  unsigned char R = (r + m) * 255;
  unsigned char G = (g + m) * 255;
  unsigned char B = (b + m) * 255;

  return SDL_Color{R, G, B, 255};
}
