#include "Textures/Particle.hpp"

Particle::Particle( int x, int y )
{
    mPosX = x - 5 + ( rand() % 25 );
    mPosY = y - 5 + ( rand() % 25 );

    mFrame = rand() % 5;

    switch( rand() % 3 )
    {
        case 0: mTexture = &gRedTexture; break;
        case 1: mTexture = &gGreenTexture; break;
        case 2: mTexture = &gBlueTexture; break;
    }
}

void Particle::init(SDL_Renderer* _gRenderer){

    this->_gRenderer = _gRenderer;

    gRedTexture.setRenderer(this->_gRenderer);
    gGreenTexture.setRenderer(this->_gRenderer);
    gBlueTexture.setRenderer(this->_gRenderer);
    // gShimmerTexture.setRenderer(this->_gRenderer);

    gRedTexture.loadFromFile( "assets/particles/minecraft_bubble_small.png");
    gRedTexture.setColor(255,0,0);
	gGreenTexture.loadFromFile( "assets/particles/minecraft_bubble_small.png");
    gGreenTexture.setColor(0,255,0);
	gBlueTexture.loadFromFile( "assets/particles/minecraft_bubble_small.png" );
    gBlueTexture.setColor(0,0,255);
	// gShimmerTexture.loadFromFile( "assets/particles/shimmer.bmp");
	// gShimmerTexture.loadFromFile( "assets/particles/minecraft_bubble_small.png");
}

void Particle::update(int x,int y){
    int ratio = 19;
    mPosX = (ratio*mPosX +x )/(ratio+1);
    mPosY = (ratio*mPosY +y)/(ratio+1);
}

void Particle::render(){
    mTexture->render( mPosX, mPosY );
    mFrame++;
}

bool Particle::isDead(){
    return mFrame > (20+rand()%8);
}
