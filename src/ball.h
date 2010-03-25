#ifndef BALL_H
#define BALL_H

#include "SDL/SDL.h"
#include "drawable.h"

class Ball {
 public:
  Ball();
  Ball(int start_x, int start_y);

  const SDL_Rect& get_position();
  void update_position(const SDL_Rect& playing_field, const SDL_Rect& first_paddle, const SDL_Rect& second_paddle);
  void set_speed(int new_x_speed, int new_y_speed);
  void draw(SDL_Surface* screen);

  static const int DefaultDiameter = 20;

 private:
  void init(int start_x, int start_y);

  SDL_Rect position;
  int x_speed;
  int y_speed;
};


#endif // BALL_H
