#pragma once

#include "iostream"
#include <stdio.h>
#include <vector>
#include "SDL2/SDL.h"
#include "math.h"

using namespace std;



class FrameGuider{
public:
    static double current_frame_double;
    static double current_frame_int;
    static double startTime;
    static double lastTime;
    static double currTime;
    const static int FRAME_RATE = 45;
    static double getFrameDelta();
    static double getFrameDeltaRounded();
    static void updateFrame();
    static void start();
};