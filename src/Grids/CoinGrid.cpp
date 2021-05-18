#include <Grids/CoinGrid.hpp>
CoinGrid* CoinGrid::_instance = nullptr;
extern AssetManager assetManager;
CoinGrid* CoinGrid::getInstance() {
  if (_instance == nullptr) {
    _instance = new CoinGrid();
  }
  return _instance;
}

CoinGrid::CoinGrid()
    : ObjectGrid(IDS::COIN_COLLIDER_ID, assetManager.get_asset(ThemeAssets::COIN_PNG), 1.2) {}

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
