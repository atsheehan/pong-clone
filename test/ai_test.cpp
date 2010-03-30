#include "unittest++/UnitTest++.h"
#include "ai.h"

namespace {

  struct AITestFixture {
    AITestFixture() {
      ball_position.x = ball_position.y = 0;
      ball_position.w = ball_position.h = 20;
      
      playing_field.x = playing_field.y = 0;
      playing_field.w = playing_field.h = 1000;
    }

    ~AITestFixture() {
    }

    SDL_Rect ball_position;
    SDL_Rect playing_field;
  };

  // the AI will move the computer paddle based on the location of the ball.
  // if the ball is in the center of the paddle, it will stay in the same spot.
  // if it is above or below the center of the paddle, it should move accordingly.

  TEST_FIXTURE(AITestFixture, keep_the_ball_in_the_center_of_the_paddle) {
    const int y_center = playing_field.y + (playing_field.h / 2);
    Paddle paddle(0, y_center - (Paddle::DefaultHeight / 2));
    AI controller(paddle);

    SDL_Rect original_position = paddle.get_position();

    // if the ball is in the center of the paddle, don't move the paddle
    ball_position.x = original_position.x + 100;
    ball_position.y = original_position.y + ((original_position.h - ball_position.h) / 2);
    
    controller.update(ball_position, playing_field);

    SDL_Rect new_position = paddle.get_position();

    CHECK(new_position.x == original_position.x);
    CHECK(new_position.y == original_position.y);
  }

  TEST_FIXTURE(AITestFixture, move_the_paddle_down_to_match_the_ball) {
    const int y_center = playing_field.y + (playing_field.h / 2);
    Paddle paddle(0, y_center - (Paddle::DefaultHeight / 2));
    AI controller(paddle);

    SDL_Rect original_position = paddle.get_position();

    // if the ball is in the center of the paddle, don't move the paddle
    ball_position.x = original_position.x + 100;
    ball_position.y = original_position.y + ((original_position.h - ball_position.h) / 2);

    // now move the ball down by three times the speed of the paddle
    ball_position.y += Paddle::DefaultSpeed * 3;

    // updating the controller should move the paddle down by its current speed    
    controller.update(ball_position, playing_field);
    SDL_Rect new_position = paddle.get_position();
    CHECK(new_position.y == original_position.y + Paddle::DefaultSpeed);

    // further updates should keep moving the paddle until it lines the center of the ball
    // with the paddle
    controller.update(ball_position, playing_field);
    controller.update(ball_position, playing_field);

    // at this point they should be squared away, but keep updating to verify that it doesnt keep
    // moving.
    controller.update(ball_position, playing_field);
    controller.update(ball_position, playing_field);

    new_position = paddle.get_position();
    CHECK(new_position.y == original_position.y + (3 * Paddle::DefaultSpeed));
  }

  // same test as earlier, move up this time
  TEST_FIXTURE(AITestFixture, move_the_paddle_up_to_match_the_ball) {
    const int y_center = playing_field.y + (playing_field.h / 2);
    Paddle paddle(0, y_center - (Paddle::DefaultHeight / 2));
    AI controller(paddle);

    SDL_Rect original_position = paddle.get_position();

    // if the ball is in the center of the paddle, don't move the paddle
    ball_position.x = original_position.x + 100;
    ball_position.y = original_position.y + ((original_position.h - ball_position.h) / 2);

    // now move the ball up by three times the speed of the paddle
    ball_position.y -= Paddle::DefaultSpeed * 3;

    // updating the controller should move the paddle up by its current speed    
    controller.update(ball_position, playing_field);
    SDL_Rect new_position = paddle.get_position();
    CHECK(new_position.y == original_position.y - Paddle::DefaultSpeed);

    // further updates should keep moving the paddle until it lines the center of the ball
    // with the paddle
    controller.update(ball_position, playing_field);
    controller.update(ball_position, playing_field);

    // at this point they should be squared away, but keep updating to verify that it doesnt keep
    // moving.
    controller.update(ball_position, playing_field);
    controller.update(ball_position, playing_field);

    new_position = paddle.get_position();
    CHECK(new_position.y == original_position.y - (3 * Paddle::DefaultSpeed));
  }
}
