#include "unittest++/UnitTest++.h"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#include "text_renderer.h"

namespace {

  struct TextRendererTestFixture {
    TextRendererTestFixture() {
      TTF_Init();
      font = TTF_OpenFont("./data/FreeMono.ttf", 20);
    }

    ~TextRendererTestFixture() {
      TTF_CloseFont(font);
      TTF_Quit();
    }

    TTF_Font* font;
  };

  // when creating a default text renderer with no arguments, it will be positioned
  // in the top left of the screen with a blank string
  TEST(default_renderer_has_no_text_and_appears_in_the_top_left_corner) {
    TextRenderer renderer;

    SDL_Rect position = renderer.get_position();
    CHECK(position.x == 0);
    CHECK(position.y == 0);

    CHECK(renderer.get_text() == "");
  }

  // since we dont know the final size of the text (it depends on the font given and
  // the point size), we specify the middle point of the text. verify that the position
  // returned is centered around the x and y values given.
  TEST_FIXTURE(TextRendererTestFixture, set_the_center_point_for_the_text_rendered) {
    TextRenderer renderer(font);

    const int center_x = 400;
    const int center_y = 200;
    renderer.set_center_position(center_x, center_y);
    renderer.set_text("hello, world");

    SDL_Rect position = renderer.get_position();
    CHECK(position.x == (center_x - (position.w / 2)));
    CHECK(position.y == (center_y - (position.h / 2)));
  }
}
