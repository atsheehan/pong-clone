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

void Ball::update_position(const SDL_Rect& playing_field, const SDL_Rect& left_paddle, const SDL_Rect& right_paddle) {

  position.x += x_speed;
  position.y += y_speed;
  
  // these calculated constants make reading the collision detection much easier
  const int playing_field_bottom_edge = playing_field.y + playing_field.h;
  const int playing_field_right_edge = playing_field.x + playing_field.w;
  const int ball_bottom_edge = position.y + position.h;
  const int ball_right_edge = position.x + position.w;
  const int left_paddle_right_edge = left_paddle.x + left_paddle.w;
  const int left_paddle_bottom_edge = left_paddle.y + left_paddle.h;
  const int right_paddle_right_edge = right_paddle.x + right_paddle.w;
  const int right_paddle_bottom_edge = right_paddle.y + right_paddle.h;
  

  // check for collision with the top and bottom of the playing field
  if (position.y < playing_field.y) {
    int difference = playing_field.y - position.y;
    position.y = playing_field.y + difference;
    y_speed = -y_speed;
  }

  if (ball_bottom_edge > playing_field_bottom_edge) {
    int difference = ball_bottom_edge - playing_field_bottom_edge;
    position.y = playing_field_bottom_edge - position.h - difference;
    y_speed = -y_speed;
  }

  // TEMPORARY FOR TESTING - this rebounds the ball from the left and right edges
  if (position.x < playing_field.x) {
    int difference = playing_field.x - position.x;
    position.x = playing_field.x + difference;
    x_speed = -x_speed;
  }

  if (ball_right_edge > playing_field_right_edge) {
    int difference = ball_right_edge - playing_field_right_edge;
    position.x = playing_field_right_edge - position.w - difference;
    x_speed = -x_speed;
  }
  // TEMPORARY

  // check for collision with the left paddle
  if (position.x < left_paddle_right_edge &&
      ball_right_edge > left_paddle.x &&
      position.y < left_paddle_bottom_edge &&
      ball_bottom_edge > left_paddle.y) {
    int difference = left_paddle_right_edge - position.x;
    position.x = left_paddle_right_edge + difference;
    x_speed = -x_speed;
  }

  // check for collision with the right paddle
  if (ball_right_edge > right_paddle.x &&
      position.x < right_paddle_right_edge &&
      position.y < right_paddle_bottom_edge &&
      ball_bottom_edge > right_paddle.y) {
    int difference = ball_right_edge - right_paddle.x;
    position.x = right_paddle.x - position.w - difference;
    x_speed = -x_speed;
  }
}

const SDL_Rect& Ball::get_position() {
  return position;
}

void Ball::set_speed(int new_x_speed, int new_y_speed) {
  x_speed = new_x_speed;
  y_speed = new_y_speed;
}

void Ball::draw(SDL_Surface* screen) {
  Uint32 white = SDL_MapRGB(screen->format, 255, 255, 255);
  SDL_FillRect(screen, &position, white);
}
