#include "Weapons/EMPBullet.hpp"

EMPBullet::EMPBullet()
    : Bullet("assets/pngs/pacman_lasers_freeze.png",
             IDS::BULLET_ID + "_" + IDS::EMP_ID + "_" + to_string(rand())) {}
