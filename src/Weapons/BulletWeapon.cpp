#include "Weapons/BulletWeapon.hpp"

#include "Network/NetworkManager.hpp"

BulletWeapon::BulletWeapon(string asset,int velocity,string id){
    this->asset = asset;
    vel = velocity;
    ColliderID = id;
    auto rect = SDL_Rect{-1, -1, 5,5};
    mCollider = Collider(ColliderID, rect);
    CollisionEngine::register_collider(&mCollider);
}

void BulletWeapon::broadcast_coordinates(){

    if(!isLaunched) return;
    Packet p;

    p.id = ColliderID;    
    p.posX = x;
    p.posY = y;
    p.velX = velX;
    p.velY = velY;
    p.data = to_string(int(angle));
    NetworkManager::queue_packet(p);


}

void BulletWeapon::receive_coordinates(){

  vector<Packet> packets;
  NetworkManager::get_packets(ColliderID, packets);

  for (auto& p : packets) {
    //   cout << "Server has got what it needs" << endl;
    isLaunched = true;
    x = p.posX;
    y = p.posY;
    velX = p.velX;
    velY = p.velY;
    angle = stoi(p.data);
  }

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

    receive_coordinates();
    broadcast_coordinates();

}

void BulletWeapon::render(){
    if(!isLaunched) return;
    


    weaponTexture.render(x,y,NULL,angle);

}