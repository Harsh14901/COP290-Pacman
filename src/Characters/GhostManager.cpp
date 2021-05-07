#include "Characters/GhostManager.hpp"


GhostManager::GhostManager(){
    // updateGhostZones();
}

void GhostManager::updateGhostZones(){ 
  string line;
  ifstream myfile("map.txt");
  int i = 0;
  int j = 0;
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      for (char& c : line) {
        if (c == 'G') {
            ghostZones.push_back(pair<int,int>(j,i));
        }else if(c == 'X'){
            ghostDoor.push_back(pair<int,int>(j,i));
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


