#include "Weapons/FreezeBullet.hpp"

extern bool is_server;

FreezeBullet::FreezeBullet()
    : Bullet("assets/pngs/pacman_lasers_freeze.png",
             IDS::BULLET_ID + "_" + IDS::FREEZEBULLET_ID + "_" +
                 to_string(rand())) {}
