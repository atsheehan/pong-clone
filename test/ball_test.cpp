#include "unittest++/UnitTest++.h"
#include "SDL/SDL.h"

#include "ball.h"

namespace {

  TEST(create_a_ball_object_using_default_values) {
    Ball default_ball = Ball();

    SDL_Rect coordinates = default_ball.get_position();

    CHECK(coordinates.x == 0);
    CHECK(coordinates.y == 0);
    CHECK(coordinates.h == Ball::DefaultDiameter);
    CHECK(coordinates.w == Ball::DefaultDiameter);
  }
}
