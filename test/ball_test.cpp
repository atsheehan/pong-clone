#include "unittest++/UnitTest++.h"
#include "SDL/SDL.h"

#include "ball.h"

namespace {

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
  TEST(a_default_ball_object_should_have_no_velocity) {
    Ball ball = Ball();
    
    // if the ball has no speed, it should remain in the same spot after an update
    SDL_Rect original_position = ball.get_position();
    ball.update_position();
    SDL_Rect new_position = ball.get_position();

    CHECK(original_position.x == new_position.x);
    CHECK(original_position.y == new_position.y);
  }

  // when the ball is assigned a velocity, verify it moves based on the values given
  // each time the update is called.
  TEST(assigning_a_velocity_to_the_ball_should_enable_it_to_move_during_an_update) {
    // set the ball at (0,0) and give is an x speed of 10 and a y speed of 20
    Ball ball = Ball();
    ball.set_speed(10, 20);

    // calling update twice should set the position at (2*10, 2*20)
    ball.update_position();
    ball.update_position();
    
    SDL_Rect new_position = ball.get_position();
    CHECK(new_position.x == 20);
    CHECK(new_position.y == 40);
  }

  // a negative speed should move the ball in the opposite direction
  TEST(verify_that_the_ball_accepts_negative_velocities) {
    Ball ball = Ball(100, 200);
    ball.set_speed(-20, -40);

    ball.update_position();
    ball.update_position();

    SDL_Rect new_position = ball.get_position();
    CHECK(new_position.x == 60);
    CHECK(new_position.y == 120);
  }

}
