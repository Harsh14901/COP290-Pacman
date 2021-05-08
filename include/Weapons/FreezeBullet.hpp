#pragma once

#include "BulletWeapon.hpp"
#include "Constants.hpp"
// #include "Characters/Pacman.hpp"
// #include "Characters/Enemy.hpp"
using namespace std;

class FreezeBullet : public BulletWeapon{
public:
    FreezeBullet(int vel);
    void checkImpact();
    void update();

};