#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "ball.h"
#include "paddle.h"
#include "score_keeper.h"

using std::cerr;
using std::endl;

int main() {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    cerr << "could not initialize SDL: " << SDL_GetError() << endl;
    return 1;
  }

  atexit(SDL_Quit);

  const int ScreenWidth = 640;
  const int ScreenHeight = 480;

  SDL_Surface* display_screen;
  display_screen = SDL_SetVideoMode(ScreenWidth, ScreenHeight, 16, SDL_SWSURFACE);
  if (display_screen == NULL) {
    cerr << "could not set the video mode: " << SDL_GetError() << endl;
    return 1;
  }

  if (TTF_Init() < 0) {
    cerr << "could not initialize the SDL ttf module: " << TTF_GetError() << endl;
    return 1;
  }
  atexit(TTF_Quit);
  
  const int FontSize = 20;
  TTF_Font* size_10_mono_font = TTF_OpenFont("./data/FreeMono.ttf", FontSize);
  if (!size_10_mono_font) {
    cerr << "could not load the font file: " << TTF_GetError() << endl;
    return 1;
  }


  const int PlayingFieldMargin = 10;
  SDL_Rect playing_field;
  playing_field.x = 0;
  playing_field.y = PlayingFieldMargin;
  playing_field.w = ScreenWidth;
  playing_field.h = ScreenHeight - (2 * PlayingFieldMargin); // add margins to the top and bottom

  SDL_Rect player_one_score_position;
  player_one_score_position.x = 40;
  player_one_score_position.y = 0;
  player_one_score_position.h = PlayingFieldMargin;
  player_one_score_position.w = 40;

  SDL_Rect player_two_score_position;
  player_two_score_position.x = 300;
  player_two_score_position.y = 0;
  player_two_score_position.h = PlayingFieldMargin;
  player_two_score_position.w = 40;
 
  ScoreKeeper player_one_score_keeper(size_10_mono_font, player_one_score_position);
  ScoreKeeper player_two_score_keeper(size_10_mono_font, player_two_score_position);

  Uint32 background_color = SDL_MapRGB(display_screen->format, 198, 226, 255);
  Uint32 playing_field_color = SDL_MapRGB(display_screen->format, 112, 128, 144);

  Ball ball(100, 100);
  ball.set_speed(10, 2);

  const int PaddleInitialY = (playing_field.h - Paddle::DefaultHeight) / 2 + playing_field.y;
  const int PaddleDistanceFromEdge = (playing_field.w - playing_field.x) / 8;
  Paddle user_paddle(playing_field.x + PaddleDistanceFromEdge, PaddleInitialY);
  Paddle computer_paddle(playing_field.x + playing_field.w - PaddleDistanceFromEdge - Paddle::DefaultWidth, PaddleInitialY);
  
  const Uint32 TicksPerFrame = 17; // 1000 ticks per second / 60 frames per second =~ 17
  Uint32 ticks_at_previous_frame = SDL_GetTicks();

  SDL_Event event;
  bool exit_game_loop = false;
  while (!exit_game_loop) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
	exit_game_loop = true;
	break;
      }
    }
    
    Uint8* keystate = SDL_GetKeyState(NULL);
    if (keystate[SDLK_w]) {
      user_paddle.move_up(playing_field);
    }

    if (keystate[SDLK_s]) {
      user_paddle.move_down(playing_field);
    }

    if (keystate[SDLK_UP]) {
      computer_paddle.move_up(playing_field);
    }

    if (keystate[SDLK_DOWN]) {
      computer_paddle.move_down(playing_field);
    }


    int result = ball.update_position(playing_field, user_paddle.get_position(), 
				      computer_paddle.get_position());
    if (result != 0) {
      ball.reset();
      ball.set_speed(10, 5);

      if (result == Ball::EnteredLeftGoal) {
	player_two_score_keeper.add_point();
      }

      if (result == Ball::EnteredRightGoal) {
	player_one_score_keeper.add_point();
      }
    }
    
    
    SDL_FillRect(display_screen, NULL, background_color);
    SDL_FillRect(display_screen, &playing_field, playing_field_color);
    ball.draw(display_screen);
    user_paddle.draw(display_screen);
    computer_paddle.draw(display_screen);
    player_one_score_keeper.draw(display_screen);
    player_two_score_keeper.draw(display_screen);
    SDL_Flip(display_screen);

    while (SDL_GetTicks() < ticks_at_previous_frame + TicksPerFrame) {}
    ticks_at_previous_frame = SDL_GetTicks();
  }

  TTF_CloseFont(size_10_mono_font);

  return 0;
}
