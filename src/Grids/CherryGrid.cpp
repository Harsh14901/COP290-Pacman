#include <Grids/CherryGrid.hpp>

void CherryGrid::generate() {
  //   if (system("node src/maze_generator.js > map.txt")) {
  //     fatalError("Error Generating map");
  //   }

  string line;
  ifstream myfile("map.txt");
  int i = 0;
  int j = 0;
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      for (char& c : line) {
        if (c == 'o') {
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
