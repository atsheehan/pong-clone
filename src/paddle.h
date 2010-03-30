#ifndef PADDLE_H
#define PADDLE_H

#include "SDL/SDL.h"
#include "drawable.h"

class Paddle : public IDrawable {
 public:
  Paddle();
  Paddle(int initial_x, int initial_y);
  const SDL_Rect& get_position();
  void move_up(const SDL_Rect& playing_field);
  void move_down(const SDL_Rect& playing_field);
  void reset_position();

  void draw(SDL_Surface* screen);

  static const int DefaultHeight = 100;
  static const int DefaultWidth = 20;
  static const int DefaultSpeed = 5;

 private:
  void init(int initial_x, int initial_y);

  SDL_Rect position;
  int speed;
  int initial_x;
  int initial_y;
};

#endif // PADDLE_H
