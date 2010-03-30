#include "paddle.h"

Paddle::Paddle() {
  init(0, 0);
}

Paddle::Paddle(int initial_x, int initial_y) {
  init(initial_x, initial_y);
}

const SDL_Rect& Paddle::get_position() {
  return position;
}

void Paddle::reset_position() {
  position.x = initial_x;
  position.y = initial_y;
}

void Paddle::move_up(const SDL_Rect& playing_field) {
  position.y -= speed;
  
  if (position.y < playing_field.y) {
    position.y = playing_field.y;
  }
}

void Paddle::move_down(const SDL_Rect& playing_field) {
  position.y += speed;
  
  const int bottom_edge_of_field = playing_field.y + playing_field.h;
  if (position.y + position.h > bottom_edge_of_field) {
    position.y = bottom_edge_of_field - position.h;
  }
}

void Paddle::init(int initial_x, int initial_y) {
  this->initial_x = position.x = initial_x;
  this->initial_y = position.y = initial_y;
  position.w = DefaultWidth;
  position.h = DefaultHeight;
  speed = DefaultSpeed;
}

void Paddle::draw(SDL_Surface* screen) {
  Uint32 white = SDL_MapRGB(screen->format, 255, 255, 255);
  SDL_FillRect(screen, &position, white);
}
