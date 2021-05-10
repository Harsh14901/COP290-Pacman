#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <functional>
#include "Collision/CollisionEngine.hpp"
#include "Constants.hpp"
#include "Textures/LTexture.hpp"

using namespace std;

class BulletWeapon{
public:
    BulletWeapon(string asset,int velocity,string id);

    void shoot(Direction dir,int x,int y);

    void setRenderer(SDL_Renderer* renderer);
    void render();


    void checkImpact();

    void update();
    void broadcast_coordinates();
    void receive_coordinates();

    Collider mCollider;
    Collider horizCollider;

    string asset;



    SDL_Renderer* renderer;

protected:
    bool isLaunched = false;
    string ColliderID;
    int x    = 0;
    int y    = 0;
    int velX = 0;
    int velY = 0;
    int vel = 0;
    LTexture weaponTexture;
    int angle = 0;

};