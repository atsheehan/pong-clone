#ifndef AI_H
#define AI_H

#include "paddle.h"
#include "SDL/SDL.h"

class AI {
 public:
  AI(Paddle& paddle);
  void update(const SDL_Rect& ball_position, const SDL_Rect& playing_field);

 private:
  Paddle& paddle;
};

#endif // AI_H
