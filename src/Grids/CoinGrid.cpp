#include <Grids/CoinGrid.hpp>

void CoinGrid::generate() {
  string line;
  ifstream myfile("map.txt");
  int i = 0;
  int j = 0;
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      for (char& c : line) {
        if (c == '.') {
          set_object(i, j);
        }
        j++;
      }
      cout << line << '\n';
      i++;
      j = 0;
    }
    myfile.close();
  }
}
