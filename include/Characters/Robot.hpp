#include "Base.hpp"
#include "Characters/Character.hpp"
using namespace std;

class Robot : public Character {
 public:
  Robot();
  void move();
  void render();
  bool is_terminated();
  void load_directions(queue<Direction>& dir_seq);
  int path_length = 0;
  int coins_collected = 0;
  int total_length = 0;

  ~Robot();

 protected:
  void init_targets();
  void target_hit(string target_id, Collider* collider = nullptr);

 private:
  queue<Direction> direction_sequence;
};