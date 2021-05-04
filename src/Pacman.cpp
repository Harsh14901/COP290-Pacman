#include "Pacman.hpp"

Pacman::Pacman() : Character(PACMAN_COLLIDER_ID) {}

extern CoinGrid coinGrid;
extern CherryGrid cherryGrid;
extern vector<Enemy> enemies;


void Pacman::handleEvent(SDL_Event& e) {
  // If a key was pressed
  if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
    // Adjust the velocity
    handle_collision();
    switch (e.key.keysym.sym) {
      case SDLK_UP:
        change_direction(Direction::UP);
        break;
      case SDLK_DOWN:
        change_direction(Direction::DOWN);
        break;
      case SDLK_LEFT:
        change_direction(Direction::LEFT);
        break;
      case SDLK_RIGHT:
        change_direction(Direction::RIGHT);
        break;
    }
  }
}

void Pacman::render() {
  // Show the dot
  int mouth_fac = isMouthOpen()?0:2;
  SDL_Rect rect{138*4 , 171 * ((2-(int(_direction)%2)) +mouth_fac ), 138, 171};
  _gDotTexture.render(mPosX, mPosY, &rect,90 * (int(_direction)/2));
}

void Pacman::handle_collision() {
  auto collisions = CollisionEngine::getCollisions(PACMAN_COLLIDER_ID);
  int i =0 ;
    // cout << "Inside pacman collision" << endl;

  // if(!collisions.empty()){
  while (i < collisions.size()) {
    if(collisions[i]->id.find(COIN_COLLIDER_ID)!=-1){
      // Coin Collected
      cout << "Coin Collected" << endl;
      i++;
      coins++;
      auto temp = extractIntegerWords(collisions[i-1]->id);
      if(temp.size()==2){
        coinGrid.unset_object(temp[0],temp[1]);
      }
      gulp_animator.start();
      continue;
    }
    if(collisions[i]->id.find(CHERRY_COLLIDER_ID)!=-1){
      cout << "Cherry Collected" << endl;
      i++;
      cherries++;
      auto temp = extractIntegerWords(collisions[i-1]->id);
      if(temp.size()==2){
        cherryGrid.unset_object(temp[0],temp[1]);
      }
      for(auto x:enemies){
        x.setState(EnemyState::WEAK);
      }
    }
    if(collisions[i]->id.find(ENEMY_COLLIDER_ID)!=-1){
      // Assert: Game Over
      is_dead = true;
      return;
    }

    // cout << "Collision of pacman with " << collisions.at(0)->id << endl;
    switch (_direction) {
      case Direction::LEFT:
      case Direction::RIGHT:
        mPosX -= 1 * mVelX;
        break;
      case Direction::UP:
      case Direction::DOWN:
        mPosY -= 1 * mVelY;
        break;
      default:
        break;
    }
    break;
  }
      // cout << "Outside pacman collision" << endl;

}

bool Pacman::isMouthOpen(){
  if(gulp_animator.isActive()){
    int cf = gulp_animator.get_current_frame();
    if(cf == 1 || cf==2){
      return false;
    }
  }
  return true;
}



void Pacman::move() {
  // Move the dot left or right
  if (!is_server) {
    handle_packets();
    return;
  }
  handle_collision();
  if (_next != Direction::NONE) {
    change_direction(_next);
  }
  mPosX += mVelX;

  // If the dot went too far to the left or right
  if ((mPosX < 0) || (mPosX + DOT_WIDTH > SCREEN_WIDTH)) {
    // Move back
    if (mPosX < 0)
      mPosX = 0;
    else
      mPosX = SCREEN_WIDTH - DOT_WIDTH;

    mVelX = 0;
  }

  // Move the dot up or down
  mPosY += mVelY;

  // If the dot went too far up or down
  if ((mPosY < 0) || (mPosY + DOT_HEIGHT > SCREEN_HEIGHT)) {
    // Move back
    if (mPosY < 0)
      mPosY = 0;
    else
      mPosY = SCREEN_HEIGHT - DOT_HEIGHT;

    mVelY = 0;
  }

  // Change rectangular collider position
  // mCollider.setX(mPosX);
  // mCollider.setY(mPosY);

  // Change circular collider position
  mCollider.setX(mPosX + PACMAN_RENDER_WIDTH / 2);
  mCollider.setY(mPosY + PACMAN_RENDER_HEIGHT / 2);
  broadcast_coordinates();
}

int Pacman::get_coins_collected(){
  return coins;
}