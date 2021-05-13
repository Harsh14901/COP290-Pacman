#include "Characters/Character.hpp"
using namespace std;

class Robot : public Character {
 public:
  Robot();
  void move();
  void render();
  bool is_terminated();
  void load_directions(queue<Direction>& dir_seq);

 private:
  queue<Direction> direction_sequence;
  bool fits_in_cell();
};