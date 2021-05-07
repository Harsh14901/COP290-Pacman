#pragma once

#include <iostream>
#include <vector>
#include "Collision/CollisionEngine.hpp"

using namespace std;

class GhostManager{
public:
    GhostManager();
    vector<pair<int,int>> ghostZones;
    vector<pair<int,int>> ghostDoor;


    void updateGhostZones();


};