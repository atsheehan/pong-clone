#include <iostream>
#include "SDL/SDL.h"

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
  }

  return 0;
}
