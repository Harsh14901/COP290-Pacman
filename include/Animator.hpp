#pragma once

class Animator {
 public:
  Animator(int sf, int dur);
  Animator(int dur);

  int start_frame = -1;
  int end_frame = -1;
  int duration;

  int get_current_frame();
  double animation_progress();
  bool isActive();
  void start();
  void reset();
};