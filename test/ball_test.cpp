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

      first_paddle.x = 40;
      second_paddle.x = 480;
      first_paddle.y = 50;
      second_paddle.y = 50;
      
      first_paddle.w = second_paddle.w = 20;
      first_paddle.h = second_paddle.h = 60;
    }

    ~BallTestFixture() {
    }
    
    SDL_Rect playing_field;
    SDL_Rect first_paddle;
    SDL_Rect second_paddle;
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
    ball.update_position(playing_field, first_paddle, second_paddle);
    SDL_Rect new_position = ball.get_position();

    CHECK(original_position.x == new_position.x);
    CHECK(original_position.y == new_position.y);
  }

  // when the ball is assigned a velocity, verify it moves based on the values given
  // each time the update is called.
  TEST_FIXTURE(BallTestFixture, assigning_a_velocity_to_the_ball_should_enable_it_to_move_during_an_update) {
    // set the ball at (0,0) and give is an x speed of 10 and a y speed of 20
    Ball ball = Ball();
    ball.set_speed(10, 20);

    // calling update twice should set the position at (2*10, 2*20)
    ball.update_position(playing_field, first_paddle, second_paddle);
    ball.update_position(playing_field, first_paddle, second_paddle);
    
    SDL_Rect new_position = ball.get_position();
    CHECK(new_position.x == 20);
    CHECK(new_position.y == 40);
  }

  // a negative speed should move the ball in the opposite direction
  TEST_FIXTURE(BallTestFixture, verify_that_the_ball_accepts_negative_velocities) {
    Ball ball = Ball(100, 200);
    ball.set_speed(-20, -40);

    ball.update_position(playing_field, first_paddle, second_paddle);
    ball.update_position(playing_field, first_paddle, second_paddle);

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

    ball.update_position(playing_field, first_paddle, second_paddle);
    SDL_Rect updated_position = ball.get_position();

    CHECK(updated_position.y == (playing_field.y + 7));

    ball.update_position(playing_field, first_paddle, second_paddle);
    updated_position = ball.get_position();

    CHECK(updated_position.y == (playing_field.y + 19));
  }

  // when the ball hits the bottom, verify that it is deflected back with the opposite velocity.
  TEST_FIXTURE(BallTestFixture, the_ball_should_deflect_off_the_bottom_of_the_playing_field) {
    // set the ball 5 units from the bottom of the field
    int ball_start_y = playing_field.y + playing_field.h - Ball::DefaultDiameter - 5;
    Ball ball = Ball(playing_field.x + 1, ball_start_y);
    
    ball.set_speed(0, 12);

    ball.update_position(playing_field, first_paddle, second_paddle);
    SDL_Rect updated_position = ball.get_position();

    CHECK(updated_position.y == (playing_field.y + playing_field.h - Ball::DefaultDiameter - 7));

    ball.update_position(playing_field, first_paddle, second_paddle);
    updated_position = ball.get_position();

    CHECK(updated_position.y == (playing_field.y + playing_field.h - Ball::DefaultDiameter - 19));
  }
}
