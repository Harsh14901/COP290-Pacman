#pragma once
#include <math.h>

#include "Constants.hpp"

enum AnimationCurve { IncreasingFreqSine };

class Animator {
 public:
  Animator(int sf, int dur);
  Animator(int dur);
  Animator();
  int start_frame = -1;
  int end_frame = -1;
  int duration;

  void set_duration(int dur);
  int get_current_frame();
  double animation_progress();
  bool isActive();
  void start();
  void reset();

  double getAnimationProgressInCurve(AnimationCurve curve, double factor,
                                     double start_offset);
};