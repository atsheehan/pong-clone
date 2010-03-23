#include "unittest++/UnitTest++.h"
#include "SDL/SDL.h"

#include "ball.h"

namespace {

  // we'll use this fixture to create the dimensions for the playing field and
  // paddles so they can be reused for testing collisions.
  struct BallTestFixture {
    BallTestFixture() {
      playing_field.x = 0;
      playing_field.y = 0;
      playing_field.w = 600;
      playing_field.h = 300;

      left_paddle.x = 40;
      right_paddle.x = 480;
      left_paddle.y = 50;
      right_paddle.y = 50;
      
      left_paddle.w = right_paddle.w = 20;
      left_paddle.h = right_paddle.h = 60;
    }

    ~BallTestFixture() {
    }
    
    SDL_Rect playing_field;
    SDL_Rect left_paddle;
    SDL_Rect right_paddle;
  };

  // although not entirely useful, if creating a ball object with no arguments
  // it should start at (0,0) with the default diameter.
  TEST(create_a_ball_object_using_default_position_values) {
    Ball default_ball = Ball();
    SDL_Rect coordinates = default_ball.get_position();

    CHECK(coordinates.x == 0);
    CHECK(coordinates.y == 0);
    CHECK(coordinates.h == Ball::DefaultDiameter);
    CHECK(coordinates.w == Ball::DefaultDiameter);
  }

  // check that supplying the starting coordinates sets the ball in the correct
  // position with the default diameter.
  TEST(create_a_ball_object_at_a_specified_location) {
    Ball ball = Ball(10, 20);
    SDL_Rect coordinates = ball.get_position();

    CHECK(coordinates.x == 10);
    CHECK(coordinates.y == 20);
    CHECK(coordinates.h == Ball::DefaultDiameter);
    CHECK(coordinates.w == Ball::DefaultDiameter);
  }
  

  // if using the defaults to create the ball object, it should start with zero velocity
  // in both the x and y directions
  TEST_FIXTURE(BallTestFixture, a_default_ball_object_should_have_no_velocity) {
    Ball ball = Ball();
    
    // if the ball has no speed, it should remain in the same spot after an update
    SDL_Rect original_position = ball.get_position();
    ball.update_position(playing_field, left_paddle, right_paddle);
    SDL_Rect new_position = ball.get_position();

    CHECK(original_position.x == new_position.x);
    CHECK(original_position.y == new_position.y);
  }

  // when the ball is assigned a velocity, verify it moves based on the values given
  // each time the update is called.
  TEST_FIXTURE(BallTestFixture, assigning_a_velocity_to_the_ball_should_enable_it_to_move_during_an_update) {
    // set the ball at (0,0) and give is an x speed of 1 and a y speed of 2
    Ball ball = Ball();
    ball.set_speed(1, 2);

    // calling update twice should set the position at (2*1, 2*2)
    ball.update_position(playing_field, left_paddle, right_paddle);
    ball.update_position(playing_field, left_paddle, right_paddle);
    
    SDL_Rect new_position = ball.get_position();
    CHECK(new_position.x == 2);
    CHECK(new_position.y == 4);
  }

  // a negative speed should move the ball in the opposite direction
  TEST_FIXTURE(BallTestFixture, verify_that_the_ball_accepts_negative_velocities) {
    Ball ball = Ball(100, 200);
    ball.set_speed(-20, -40);

    ball.update_position(playing_field, left_paddle, right_paddle);
    ball.update_position(playing_field, left_paddle, right_paddle);

    SDL_Rect new_position = ball.get_position();
    CHECK(new_position.x == 60);
    CHECK(new_position.y == 120);
  }

  // when the ball hits the top, verify that it is deflected back with the opposite velocity.
  TEST_FIXTURE(BallTestFixture, the_ball_should_deflect_off_the_top_of_the_playing_field) {
    // set the ball 5 units from the top of the field
    Ball ball = Ball(playing_field.x + 1, playing_field.y + 5);
    
    // the ball should move up 12 units each update. the first time it moves, it will hit the wall
    // which is 5 units away, and then rebound 7 units back. the second time it moves, the speed
    // should have been reversed so that it moved 12 units down, ending up 19 units from the top
    // of the wall.
    ball.set_speed(0, -12);

    ball.update_position(playing_field, left_paddle, right_paddle);
    SDL_Rect updated_position = ball.get_position();

    CHECK(updated_position.y == (playing_field.y + 7));

    ball.update_position(playing_field, left_paddle, right_paddle);
    updated_position = ball.get_position();

    CHECK(updated_position.y == (playing_field.y + 19));
  }

  // when the ball hits the bottom, verify that it is deflected back with the opposite velocity.
  TEST_FIXTURE(BallTestFixture, the_ball_should_deflect_off_the_bottom_of_the_playing_field) {
    // set the ball 5 units from the bottom of the field
    int ball_start_y = playing_field.y + playing_field.h - Ball::DefaultDiameter - 5;
    Ball ball = Ball(playing_field.x + 1, ball_start_y);
    
    ball.set_speed(0, 12);

    ball.update_position(playing_field, left_paddle, right_paddle);
    SDL_Rect updated_position = ball.get_position();

    CHECK(updated_position.y == (playing_field.y + playing_field.h - Ball::DefaultDiameter - 7));

    ball.update_position(playing_field, left_paddle, right_paddle);
    updated_position = ball.get_position();

    CHECK(updated_position.y == (playing_field.y + playing_field.h - Ball::DefaultDiameter - 19));
  }
  
  // when the ball hits the front of the left paddle, it should deflect back with the same
  // speed.
  TEST_FIXTURE(BallTestFixture, deflect_off_front_of_left_paddle) {
    // start the ball 5 units away from the front of the left (first) paddle
    const int right_edge_of_paddle = left_paddle.x + left_paddle.w;
    int ball_start_x = right_edge_of_paddle + 5;
    Ball ball(ball_start_x, left_paddle.y);

    // if the ball is moving towards the paddle at 12 units, it should deflect back after
    // hitting it at 5 units and then move 7 units away from the start. after another update
    // it should move 12 units further away and end up at 19 units from the paddle.
    ball.set_speed(-12, 0);
    ball.update_position(playing_field, left_paddle, right_paddle);
    ball.update_position(playing_field, left_paddle, right_paddle);

    SDL_Rect new_position = ball.get_position();
    CHECK(new_position.x == right_edge_of_paddle + 19);
  }

  // when testing for a collision with the paddle, need to check whether the x-value extends
  // past the right edge of the paddle as well as checking whether the y-values are within the
  // bounds of the paddle.
  TEST_FIXTURE(BallTestFixture, move_past_left_paddle_if_not_within_y_coordinates) {
    // start the ball 5 units away from the front of the left paddle, but move it
    // so that it should avoid hitting the bottom of the paddle
    const int right_edge_of_paddle = left_paddle.x + left_paddle.w;
    const int bottom_edge_of_paddle = left_paddle.y + left_paddle.h;
    Ball ball(right_edge_of_paddle + 5, bottom_edge_of_paddle);

    // the ball should move past the paddle without colliding or changing direction. starting
    // at 5 units to the right of the paddle, after updating twice at a speed of 12 it should be
    // (5 - 2*12) = -19 units away from the right edge of the paddle
    ball.set_speed(-12, 0);
    ball.update_position(playing_field, left_paddle, right_paddle);
    ball.update_position(playing_field, left_paddle, right_paddle);

    SDL_Rect new_position = ball.get_position();
    CHECK(new_position.x == right_edge_of_paddle - 19);    
  }

  // if the ball moves past the paddle and is moving up behind it, it should not deflect off 
  // anything.
  TEST_FIXTURE(BallTestFixture, move_behind_left_paddle) {
    const int paddle_bottom_edge = left_paddle.y + left_paddle.h;
    const int ball_start_x = left_paddle.x - Ball::DefaultDiameter;
    Ball ball(ball_start_x, paddle_bottom_edge + 5);

    // move the ball up behind the paddle at 12 units / update while moving slightly to the left
    // at 1 unit/update. after two updates, the top edge of the ball should be 19 units past the
    // bottom edge of the paddle.
    ball.set_speed(-1, -12);
    ball.update_position(playing_field, left_paddle, right_paddle);
    ball.update_position(playing_field, left_paddle, right_paddle);
    
    SDL_Rect new_position = ball.get_position();
    CHECK(new_position.y == paddle_bottom_edge - 19);
    CHECK(new_position.x == ball_start_x - 2);
  }

  // when the ball hits the front of the right paddle, it should deflect back with the same
  // speed.
  TEST_FIXTURE(BallTestFixture, deflect_off_front_of_right_paddle) {
    // start the ball 5 units away from the front of the right paddle
    int ball_start_x = right_paddle.x - Ball::DefaultDiameter - 5;
    Ball ball(ball_start_x, right_paddle.y);

    // if the ball is moving towards the paddle at 12 units, it should deflect back after
    // hitting it at 5 units and then move 7 units away from the start. after another update
    // it should move 12 units further away and end up at 19 units from the paddle.
    ball.set_speed(12, 0);
    ball.update_position(playing_field, left_paddle, right_paddle);
    ball.update_position(playing_field, left_paddle, right_paddle);

    SDL_Rect new_position = ball.get_position();
    CHECK(new_position.x == right_paddle.x - Ball::DefaultDiameter - 19);
  }

  // when testing for a collision with the paddle, need to check whether the x-value extends
  // past the edge of the paddle as well as checking whether the y-values are within the
  // bounds of the paddle.
  TEST_FIXTURE(BallTestFixture, move_past_right_paddle_if_not_within_y_coordinates) {
    // start the ball 5 units away from the front of the right paddle, but move it
    // so that it should avoid hitting the bottom of the paddle
    int ball_start_x = right_paddle.x - Ball::DefaultDiameter - 5;
    Ball ball(ball_start_x, right_paddle.y + right_paddle.h);

    // the ball should move past the paddle without colliding or changing direction. starting
    // at 5 units away from the edge of the paddle, after updating twice at a speed of 12 the right
    // edge of the ball  should be (-5 + 2*12) = 19 units away from the left edge of the paddle
    ball.set_speed(12, 0);
    ball.update_position(playing_field, left_paddle, right_paddle);
    ball.update_position(playing_field, left_paddle, right_paddle);

    SDL_Rect new_position = ball.get_position();
    CHECK(new_position.x + new_position.w == right_paddle.x + 19);    
  }

  // if the ball moves past the paddle and is moving up behind it, it should not deflect off 
  // anything.
  TEST_FIXTURE(BallTestFixture, move_behind_right_paddle) {
    const int paddle_bottom_edge = right_paddle.y + right_paddle.h;
    const int ball_start_x = right_paddle.x + right_paddle.w;
    Ball ball(ball_start_x, paddle_bottom_edge + 5);

    // move the ball up behind the paddle at 12 units / update while moving slightly to the right
    // at 1 unit/update. after two updates, the top edge of the ball should be 19 units past the
    // bottom edge of the paddle.
    ball.set_speed(1, -12);
    ball.update_position(playing_field, left_paddle, right_paddle);
    ball.update_position(playing_field, left_paddle, right_paddle);
    
    SDL_Rect new_position = ball.get_position();
    CHECK(new_position.y == paddle_bottom_edge - 19);
    CHECK(new_position.x == ball_start_x + 2);
  }
}
