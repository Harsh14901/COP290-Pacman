#include "Utils/Animator.hpp"

#include <iostream>
using namespace std;

Animator::Animator(int sf, int dur) {
  start_frame = sf;
  end_frame = sf + dur;
  duration = dur;
}

Animator::Animator(int dur) { duration = dur; }
Animator::Animator() : Animator(0){};
void Animator::start() {
  start_frame = game_frame_int;
  end_frame = start_frame + duration;
}

void Animator::reset() {
  // Dont reset the duration
  start_frame = -1;
  end_frame = -1;
}

void Animator::set_duration(int dur) { duration = dur; }

int Animator::get_current_frame() {
  if (!isActive()) return -1;
  return game_frame_int - start_frame;
}

double Animator::animation_progress() {
  if (!isActive()) return -1;
  return (get_current_frame()) / (0.0 + end_frame - start_frame);
}

double Animator::getAnimationProgressInCurve(AnimationCurve curve,
                                             double factor,
                                             double start_offset) {
  if (!isActive()) return -99;

  if (curve == AnimationCurve::IncreasingFreqSine) {
    double x =
        (game_frame / (end_frame - start_frame + 0.0) - start_offset) * factor;
    return sin(exp(x));
  }
  return -99;
}

bool Animator::isActive() { return game_frame_int < end_frame; }