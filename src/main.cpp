#include <iostream>
#include "SDL/SDL.h"
#include "ball.h"
#include "paddle.h"

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

  const int PlayingFieldMargin = 10;
  SDL_Rect playing_field;
  playing_field.x = 0;
  playing_field.y = PlayingFieldMargin;
  playing_field.w = ScreenWidth;
  playing_field.h = ScreenHeight - (2 * PlayingFieldMargin); // add margins to the top and bottom

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
    }
    
    
    SDL_FillRect(display_screen, NULL, background_color);
    SDL_FillRect(display_screen, &playing_field, playing_field_color);
    ball.draw(display_screen);
    user_paddle.draw(display_screen);
    computer_paddle.draw(display_screen);
    SDL_Flip(display_screen);

    while (SDL_GetTicks() < ticks_at_previous_frame + TicksPerFrame) {}
    ticks_at_previous_frame = SDL_GetTicks();
  }

  return 0;
}
