#include "Weapons/BulletWeapon.hpp"


BulletWeapon::BulletWeapon(string asset,int velocity,string id){
    this->asset = asset;
    vel = velocity;
    ColliderID = id + to_string(rand()%10000);
    auto rect = SDL_Rect{-1, -1, 5,5};
    mCollider = Collider(ColliderID, rect);
    CollisionEngine::register_collider(&mCollider);
}


void BulletWeapon::shoot(Direction dir,int x,int y){
    this->x = x;
    this->y = y;
    if(dir==Direction::DOWN){
        velX = 0;
        velY = vel;
        angle = 180;
    }else if(dir==Direction::UP){
        velX = 0;
        velY = -vel;
        angle = 0;
    }else if(dir==Direction::LEFT){
        velX =-vel;
        velY = 0;
        angle = -90;
    }else{
        velY = 0;
        velX = vel;
        angle = 90;
    }
    cout << "Launching bullet" << endl;
    isLaunched = true;
}

void BulletWeapon::setRenderer(SDL_Renderer* renderer){
    this->renderer = renderer;
    weaponTexture.setRenderer(renderer);
    weaponTexture.loadFromFile(asset);
    weaponTexture.set_image_dimenstions(25,39);
}

void BulletWeapon::checkImpact(){

}




void BulletWeapon::update(){
    if(!isLaunched) {
        cout << "Not Launched" << endl;
        return;
    }

    x += velX;
    y += velY;

    mCollider.setX(x);
    mCollider.setY(y);

    checkImpact();

}

void BulletWeapon::render(){
    if(!isLaunched) return;
    


    weaponTexture.render(x,y,NULL,angle);

}