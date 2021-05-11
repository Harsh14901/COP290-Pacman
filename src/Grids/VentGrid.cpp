#include <Grids/VentGrid.hpp>
#include "math.h"
VentGrid* VentGrid::_instance = nullptr;


// extern WallGrid wallGrid;

VentGrid* VentGrid::getInstance() {
  if (_instance == nullptr) {
    _instance = new VentGrid();
  }
  return _instance;
}

VentGrid::VentGrid()
    : ObjectGrid(IDS::VENT_COLLIDER_ID, "assets/pngs/amongus_vent.png", 1) {

    }

// Assumption: Wall Grid is initialised
void VentGrid::generate() {

    auto wallGrid = WallGrid::getInstance();
    // int rows = sizeof(walls)/sizeof(walls[0]);
    // int cols = sizeof(walls[0])/sizeof(walls[0][0]);
    int rows = GRID_ROW;
    int cols = GRID_COL;

    cout << "Rows & cols are " << rows << "," << cols << endl;

    bool flag = false;

    for(int x=0;x<rows*cols;x++){
        if(flag) break;
        for(int i=0;i<=x;i++){
            int j = x-i;
            if(wallGrid->is_empty(i,j)){
                set_object(i,j);
                vents.push_back({i,j});
                flag = true;
                break;
            }
        }
    }

    flag = false;

    for(int x=0;x<rows*cols;x++){
        if(flag) break;
        for(int i=0;i<=x;i++){
            int j = cols - (x-i) -1;
            if(wallGrid->is_empty(i,j)){
                set_object(i,j);
                vents.push_back({i,j});
                flag = true;
                break;
            }
        }
    }


    cout << "Completed 2" << endl;

    flag = false;


    for(int x=0;x<rows*cols;x++){
        if(flag) break;
        for(int j=0;j<=x;j++){
            int i = rows - (x-j) - 1;
            if(wallGrid->is_empty(i,j)){
                set_object(i,j);
                vents.push_back({i,j});
                flag = true;
                break;
            }
        }
    }

    flag = false;


    for(int x=0;x<rows*cols;x++){
        if(flag) break;
        for(int i=0;i<=x;i++){
            int j = cols - (x-i) - 1;
            int i_dash =  rows - (x-i) - 1;
            if(wallGrid->is_empty(i_dash,j)){
                set_object(i_dash,j);
                vents.push_back({i,j});
                flag = true;
                break;
            }
        }
    }

}

bool VentGrid::handleOpening(int i,int j){
    openingAnimator.start();
    for (auto x: vents){
        if(abs(x.first-j)<2 && abs(x.second-i)<2){
            currentVent = pair<int,int>(j,i);
            isOpen = true;
            return true;
            break;
        }
    }   
    return false;
}

void VentGrid::teleportRandom(){
    if(!isOpen) return;
    int x = 0;
    while(true){
        x = rand()%4;
        if(vents[x].first==currentVent.first && vents[x].second==currentVent.second) continue;
        break;
    }
    // isOpen = false;
    teleportLocation = vents[x];
}

bool VentGrid::canTeleport(){
    return isOpen && openingAnimator.animation_progress()>0.1;
}

pair<int,int> VentGrid::getTeleportLocation(){
    teleportRandom();
    isOpen = false;
    return pair<int,int>(teleportLocation.second*32,teleportLocation.first*32);
}

SDL_Rect VentGrid::getRect(int i,int j){
    SDL_Rect rect = {0, 0, 63, 71};
    // if(openingAnimator.get_current_frame()==5){
    //     teleportRandom();
    // }
    if(isOpen){// && currentVent.first==i && currentVent.second==j){
        rect = {63*  (1+openingAnimator.get_current_frame()<3?0:1) ,0,63,71};
    }
    return rect;
}


void VentGrid::render() {
  // cout << "Rendering Objects" << endl;
  int dir = 0;
  for (int i = 0; i < GRID_ROW; i++) {
    for (int j = 0; j < GRID_COL; j++) {
      if (objects[i][j]) {
        auto rect = getRect(i,j);
        object_texture.render(
            j * WALL_GRID_WIDTH + (WALL_GRID_WIDTH - OBJECT_WIDTH) / 2,
            i * WALL_GRID_HEIGHT + (WALL_GRID_HEIGHT - OBJECT_HEIGHT) / 2,&rect,90*dir);
      }
    }
  }
}