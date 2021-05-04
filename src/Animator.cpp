#include "Animator.hpp"
#include <iostream>
using namespace std;

extern int game_frame;

Animator::Animator(int sf,int dur){
    start_frame = sf;
    end_frame = sf+dur;
    duration  = dur;
}

Animator::Animator(int dur){
    duration = dur;
}

void Animator::start(){
    start_frame = game_frame;
    end_frame = start_frame+duration;
}

void Animator::reset(){
    // Dont reset the duration
    start_frame = -1;
    end_frame = -1;
}



int Animator::get_current_frame(){
    if(!isActive()) return -1;
    return game_frame - start_frame;
}

double Animator::animation_progress(){
    if(!isActive()) return -1;
    return (get_current_frame())/(0.0+end_frame-start_frame);
}

bool Animator::isActive(){
    return game_frame < end_frame;
}