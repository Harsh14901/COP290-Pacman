#pragma once


#include "iostream"
#include "Constants.hpp"
#include <stdlib.h> /* srand, rand */
#include <bits/stdc++.h>
#include "Grids/WallGrid.hpp"

using namespace std;


class GhostAIEngine{
public:
    void init(int DOT_WIDTH,int DOT_HEIGHT,int type);
    void update(int mPosX,int mPosY,Direction _direction,EnemyState enemyState);  
    Direction updateDirection();
    int calcDirectionScore(int targetX,int targetY,Direction dir);
    Direction findBestDirection(int targetX,int targetY,unordered_set<Direction> available_directions);
    Direction reverseDirectionIfNecessary(Direction dir);
    int directionToInt(Direction dir);
    int mPosX;
    int mPosY;
    int DOT_WIDTH;
    int DOT_HEIGHT;
    int type;
    Direction _direction;
    EnemyState enemyState;


};