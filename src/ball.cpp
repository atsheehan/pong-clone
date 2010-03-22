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

  x_speed = y_speed = 0;
}

void Ball::update_position(const SDL_Rect& playing_field, const SDL_Rect& first_paddle, const SDL_Rect& second_paddle) {

  position.x += x_speed;
  position.y += y_speed;
  
  if (position.y < playing_field.y) {
    int difference = playing_field.y - position.y;
    position.y = playing_field.y + difference;
    y_speed = -y_speed;
  }

  const int playing_field_bottom_edge = playing_field.y + playing_field.h;
  const int ball_bottom_edge = position.y + position.h;

  if (ball_bottom_edge > playing_field_bottom_edge) {
    int difference = ball_bottom_edge - playing_field_bottom_edge;
    position.y = playing_field_bottom_edge - position.h - difference;
    y_speed = -y_speed;
  }
}

SDL_Rect Ball::get_position() {
  return position;
}

void Ball::set_speed(int new_x_speed, int new_y_speed) {
  x_speed = new_x_speed;
  y_speed = new_y_speed;
}
