#include "unittest++/UnitTest++.h"
#include "SDL/SDL.h"

#include "text_renderer.h"

namespace {

  // when creating a default text renderer with no arguments, it will be positioned
  // in the top left of the screen with a blank string
  TEST(default_renderer_has_no_text_and_appears_in_the_top_left_corner) {
    TextRenderer renderer;

    SDL_Rect position = renderer.get_position();
    CHECK(position.x == 0);
    CHECK(position.y == 0);

    CHECK(renderer.get_text() == "");
  }

}
