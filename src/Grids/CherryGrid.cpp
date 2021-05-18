#include <Grids/CherryGrid.hpp>
unique_ptr<CherryGrid> CherryGrid::_instance;
CherryGrid* CherryGrid::getInstance() {
  if (_instance == nullptr) {
    _instance = make_unique<CherryGrid>();
  }
  return _instance.get();
}

CherryGrid::CherryGrid()
    : ObjectGrid(IDS::CHERRY_COLLIDER_ID,
                 AssetManager::get_asset(ThemeAssets::CHERRY_PNG), 1.2) {}

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
