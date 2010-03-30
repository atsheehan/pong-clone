#include "ai.h"

AI::AI(Paddle& paddle) : paddle(paddle) {
}

void AI::update(const SDL_Rect& ball_position, const SDL_Rect& playing_field) {
  // calculate the center of the paddle and of the ball
  SDL_Rect paddle_position = paddle.get_position();
  const int paddle_center = paddle_position.y + (paddle_position.h / 2);
  const int ball_center = ball_position.y + (ball_position.h / 2);

  // adjust the paddle accordingly to line the center of the paddle up with the center of the ball
  if (paddle_center < ball_center) {
    paddle.move_down(playing_field);
  }

  if (paddle_center > ball_center) {
    paddle.move_up(playing_field);
  }
  
}
