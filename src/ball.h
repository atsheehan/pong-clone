#ifndef BALL_H
#define BALL_H

#include "SDL/SDL.h"

class Ball {
 public:
  Ball();

  SDL_Rect get_position();
  static const int DefaultDiameter = 20;
};


#endif // BALL_H
