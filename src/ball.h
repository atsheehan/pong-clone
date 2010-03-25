#ifndef BALL_H
#define BALL_H

#include "SDL/SDL.h"
#include "drawable.h"

class Ball : public IDrawable {
 public:
  Ball();
  Ball(int start_x, int start_y);

  const SDL_Rect& get_position();
  int update_position(const SDL_Rect& playing_field, const SDL_Rect& first_paddle, const SDL_Rect& second_paddle);
  void set_speed(int new_x_speed, int new_y_speed);
  void reset();
  void draw(SDL_Surface* screen);

  static const int DefaultDiameter = 20;
  static const int EnteredLeftGoal = 1;
  static const int EnteredRightGoal = 2;

 private:
  void init(int start_x, int start_y);

  SDL_Rect position;
  int x_speed;
  int y_speed;
  int initial_x;
  int initial_y;
};


#endif // BALL_H
