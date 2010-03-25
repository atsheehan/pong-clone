#include "unittest++/UnitTest++.h"
#include "SDL/SDL.h"

#include "paddle.h"

namespace {

  struct PaddleTestFixture {
    PaddleTestFixture() {
      playing_field.x = 0;
      playing_field.y = 0;
      playing_field.h = 1000;
      playing_field.w = 2000;
    }

    ~PaddleTestFixture() {}

    SDL_Rect playing_field;
  };

  TEST(default_paddle_object_should_start_at_zero_zero_with_the_default_size) {
    Paddle default_paddle;
    
    SDL_Rect position = default_paddle.get_position();
    CHECK(position.x == 0);
    CHECK(position.y == 0);
    CHECK(position.w == Paddle::DefaultWidth);
    CHECK(position.h == Paddle::DefaultHeight);
  }

  TEST(supplying_the_paddle_object_with_an_initial_position) {
    Paddle paddle(100, 200);

    SDL_Rect position = paddle.get_position();
    CHECK(position.x == 100);
    CHECK(position.y == 200);
    CHECK(position.h == Paddle::DefaultHeight);
    CHECK(position.w == Paddle::DefaultWidth);
  }

  TEST_FIXTURE(PaddleTestFixture, move_the_paddle_up_by_the_default_speed) {
    Paddle paddle(100, 200);

    paddle.move_up(playing_field);
    SDL_Rect position = paddle.get_position();

    CHECK(position.y == 200 - Paddle::DefaultSpeed);
    CHECK(position.x == 100);
  }

  TEST_FIXTURE(PaddleTestFixture, move_the_paddle_down_by_the_default_speed) {
    Paddle paddle(100, 200);

    paddle.move_down(playing_field);
    SDL_Rect position = paddle.get_position();

    CHECK(position.y == 200 + Paddle::DefaultSpeed);
    CHECK(position.x == 100);
  }

  TEST_FIXTURE(PaddleTestFixture, stop_paddle_from_moving_past_the_edge_of_the_field) {
    Paddle paddle_up(100, 2);
    paddle_up.move_up(playing_field);
    SDL_Rect new_position = paddle_up.get_position();
    CHECK(new_position.y == 0);
    paddle_up.move_up(playing_field);
    new_position = paddle_up.get_position();
    CHECK(new_position.y == 0);

    Paddle paddle_down(100, playing_field.h - Paddle::DefaultHeight - 2);
    paddle_down.move_down(playing_field);
    new_position = paddle_down.get_position();
    CHECK(new_position.y == playing_field.h - Paddle::DefaultHeight);
    paddle_down.move_down(playing_field);
    new_position = paddle_down.get_position();
    CHECK(new_position.y == playing_field.h - Paddle::DefaultHeight);
  }
}
