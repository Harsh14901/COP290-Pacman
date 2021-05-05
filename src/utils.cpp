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