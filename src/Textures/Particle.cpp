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
    gShimmerTexture.setRenderer(this->_gRenderer);

    gRedTexture.loadFromFile( "assets/particles/red.bmp");
	gGreenTexture.loadFromFile( "assets/particles/green.bmp");
	gBlueTexture.loadFromFile( "assets/particles/blue.bmp" );
	gShimmerTexture.loadFromFile( "assets/particles/shimmer.bmp");
}

void Particle::render(){
	mTexture->render( mPosX, mPosY );

    if( mFrame % 2 == 0 )
    {
		gShimmerTexture.render( mPosX, mPosY );
    }

    mFrame++;
}

bool Particle::isDead(){
    return mFrame > 10;
}
