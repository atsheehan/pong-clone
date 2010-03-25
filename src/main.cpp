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

  SDL_Surface* display_screen;
  display_screen = SDL_SetVideoMode(400, 400, 16, SDL_SWSURFACE);
  if (display_screen == NULL) {
    cerr << "could not set the video mode: " << SDL_GetError() << endl;
    return 1;
  }

  SDL_Rect playing_field;
  playing_field.x = playing_field.y = 0;
  playing_field.h = playing_field.w = 400;

  Ball ball(100, 100);
  ball.set_speed(10, 2);

  Paddle user_paddle(40, 0);
  Paddle computer_paddle(330, 0);
  
  Uint32 black = SDL_MapRGB(display_screen->format, 0, 0, 0);
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
    if (keystate[SDLK_UP]) {
      user_paddle.move_up(playing_field);
    }

    if (keystate[SDLK_DOWN]) {
      user_paddle.move_down(playing_field);
    }


    ball.update_position(playing_field, user_paddle.get_position(), computer_paddle.get_position());
    
    SDL_FillRect(display_screen, NULL, black);
    ball.draw(display_screen);
    user_paddle.draw(display_screen);
    computer_paddle.draw(display_screen);
    SDL_Flip(display_screen);

    while (SDL_GetTicks() < ticks_at_previous_frame + TicksPerFrame) {}
    ticks_at_previous_frame = SDL_GetTicks();
  }

  return 0;
}
