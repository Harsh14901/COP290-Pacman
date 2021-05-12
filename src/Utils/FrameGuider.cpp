#include "Utils/FrameGuider.hpp"

double FrameGuider::current_frame_double = 0;
double FrameGuider::current_frame_int = 0;
double FrameGuider::startTime = 0;
double FrameGuider::lastTime = -1;
double FrameGuider::currTime = 0;



void FrameGuider::start(){
    startTime = double(SDL_GetTicks());
    currTime = startTime;
}

void FrameGuider::updateFrame(){
    lastTime = currTime;
    currTime = double(SDL_GetTicks());
    current_frame_double = (currTime - startTime)*FRAME_RATE/1000;
    current_frame_int = int(round(current_frame_double));
}

double FrameGuider::getFrameDelta(){
    cout << (currTime - lastTime)*FRAME_RATE/1000 << endl;
    return (currTime - lastTime)*FRAME_RATE/1000;
}

double FrameGuider::getFrameDeltaRounded(){
    return 1;
    // return min(1.0,round((currTime - lastTime)*FRAME_RATE/1000));
}

