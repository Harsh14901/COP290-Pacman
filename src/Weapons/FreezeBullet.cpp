#include "Weapons/FreezeBullet.hpp"
#include "Characters/Pacman.hpp"
// extern Pacman pacman;

extern bool is_server;

void FreezeBullet::checkImpact(){
    auto collisions = CollisionEngine::getCollisions(ColliderID);

    for(auto item: collisions){
        if(item->id.find(IDS::WALL_COLLIDER_ID)!=-1){
            cout << "Collision With Wall" << endl;
            // isLaunched = false;
            // continue;
        }
    }
}

FreezeBullet::FreezeBullet(int vel) : BulletWeapon("assets/pngs/pacman_lasers_freeze.png",vel,IDS::FREEZEBULLET_ID){


}

void FreezeBullet::update(){
    
    if(is_server){
        // cout << "Receiving coords" << endl;
        BulletWeapon::receive_coordinates();
    }

    if(!isLaunched) return;


    x += velX;
    y += velY;

    mCollider.setX(x);
    mCollider.setY(y);

    checkImpact();

    if(!is_server){
        // cout << "Broadcasting Coordis" << endl;
        BulletWeapon::broadcast_coordinates();
    }



}