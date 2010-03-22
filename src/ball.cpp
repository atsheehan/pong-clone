#include "ball.h"

Ball::Ball() {
  init(0, 0);
}

Ball::Ball(int start_x, int start_y) {
  init(start_x, start_y);
}

void Ball::init(int start_x, int start_y) {
  position.x = start_x;
  position.y = start_y;

  position.h = position.w = DefaultDiameter;
}

void Ball::update_position() {
  position.x += x_speed;
  position.y += y_speed;
}

SDL_Rect Ball::get_position() {
  return position;
}

void Ball::set_speed(int new_x_speed, int new_y_speed) {
  x_speed = new_x_speed;
  y_speed = new_y_speed;
}
