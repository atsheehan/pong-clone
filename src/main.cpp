#include <iostream>
#include <vector>
#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "ball.h"
#include "paddle.h"
#include "score_keeper.h"
#include "menu.h"
#include "ai.h"

using std::cerr;
using std::endl;
using std::vector;
using std::string;

int main() {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    cerr << "could not initialize SDL: " << SDL_GetError() << endl;
    return 1;
  }

  atexit(SDL_Quit);

  const int ScreenWidth = 800;
  const int ScreenHeight = 600;

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
  TTF_Font* size_10_mono_font = TTF_OpenFont("./data/FreeMonoBold.ttf", FontSize);
  if (!size_10_mono_font) {
    cerr << "could not load the font file: " << TTF_GetError() << endl;
    return 1;
  }

  const int NewGameSelection = 0;
  const int ResumeSelection = 1;
  const int QuitSelection = 2;
  vector<string> menu_selections;
  menu_selections.push_back("new game");
  menu_selections.push_back("resume");
  menu_selections.push_back("quit");
  Menu menu("pongo", menu_selections, 100, ScreenWidth / 2, size_10_mono_font);

  const int MenuState = 1;
  const int RunState = 2;
  int current_game_state = MenuState;

  const int PlayingFieldMargin = 30;
  SDL_Rect playing_field;
  playing_field.x = 0;
  playing_field.y = PlayingFieldMargin;
  playing_field.w = ScreenWidth;
  playing_field.h = ScreenHeight - (2 * PlayingFieldMargin); // add margins to the top and bottom

  const int ScoreBoxWidth = 40;
  SDL_Rect player_one_score_position;
  player_one_score_position.x = playing_field.x + ((playing_field.w / 4) - (ScoreBoxWidth / 2));
  player_one_score_position.y = 0;
  player_one_score_position.h = PlayingFieldMargin;
  player_one_score_position.w = ScoreBoxWidth;

  SDL_Rect player_two_score_position;
  player_two_score_position.x = playing_field.x + ((3 * playing_field.w / 4) - (ScoreBoxWidth / 2));
  player_two_score_position.y = 0;
  player_two_score_position.h = PlayingFieldMargin;
  player_two_score_position.w = ScoreBoxWidth;
 
  ScoreKeeper player_one_score_keeper(size_10_mono_font, player_one_score_position);
  ScoreKeeper player_two_score_keeper(size_10_mono_font, player_two_score_position);

  Uint32 background_color = SDL_MapRGB(display_screen->format, 198, 226, 255);
  Uint32 playing_field_color = SDL_MapRGB(display_screen->format, 112, 128, 144);

  const int InitialBallXPosition = playing_field.x + (playing_field.w - Ball::DefaultDiameter) / 2;
  const int InitialBallYPosition = playing_field.y + (playing_field.h - Ball::DefaultDiameter) / 2;
  Ball ball(InitialBallXPosition, InitialBallYPosition);

  const int PaddleInitialY = (playing_field.h - Paddle::DefaultHeight) / 2 + playing_field.y;
  const int PaddleDistanceFromEdge = (playing_field.w - playing_field.x) / 8;
  Paddle user_paddle(playing_field.x + PaddleDistanceFromEdge, PaddleInitialY);
  Paddle computer_paddle(playing_field.x + playing_field.w - PaddleDistanceFromEdge - Paddle::DefaultWidth, PaddleInitialY);

  AI controller(computer_paddle);
  
  const Uint32 TicksPerFrame = 17; // 1000 ticks per second / 60 frames per second =~ 17
  Uint32 ticks_at_previous_frame = SDL_GetTicks();

  SDL_Event event;
  bool exit_game_loop = false;
  while (!exit_game_loop) {
    int result;
    Uint8* keystate;
    switch (current_game_state) {

    case MenuState:
      while (SDL_PollEvent(&event)) {
	switch (event.type) {
	case SDL_QUIT:
	  exit_game_loop = true;
	  break;

	case SDL_KEYDOWN:
	  switch (event.key.keysym.sym) {
	  case SDLK_DOWN:
	    menu.next_selection();
	    break;

	  case SDLK_UP:
	    menu.previous_selection();
	    break;

	  case SDLK_RETURN:
	    int current_selection = menu.get_current_selection();
	    switch (current_selection) {
	    case NewGameSelection:
	      user_paddle.reset_position();
	      computer_paddle.reset_position();
	      player_one_score_keeper.reset_points();
	      player_two_score_keeper.reset_points();
	      ball.reset();
	      current_game_state = RunState;
	      break;

	    case ResumeSelection:
	      current_game_state = RunState;
	      break;

	    case QuitSelection:
	      exit_game_loop = true;
	      break;

	    default:
	      break;
	    }
	    break;
	  }
	}
      }

      SDL_FillRect(display_screen, NULL, background_color);
      SDL_FillRect(display_screen, &playing_field, playing_field_color);
      menu.draw(display_screen);
      break;

    case RunState:
      while (SDL_PollEvent(&event)) {
	switch (event.type) {
	case SDL_QUIT:
	  exit_game_loop = true;
	  break;

	case SDL_KEYDOWN:
	  switch (event.key.keysym.sym) {
	  case SDLK_SPACE:
	    if (!ball.is_in_motion()) {
	      ball.set_speed(10, 5);
	    }
	    break;

	  case SDLK_ESCAPE:
	    current_game_state = MenuState;
	    break;
	  }
	  break;


	}
      }
    
      keystate = SDL_GetKeyState(NULL);
      if (keystate[SDLK_UP]) {
	user_paddle.move_up(playing_field);
      }

      if (keystate[SDLK_DOWN]) {
	user_paddle.move_down(playing_field);
      }


      result = ball.update_position(playing_field, user_paddle.get_position(), 
				    computer_paddle.get_position());
      if (result != 0) {
	ball.reset();
	if (result == Ball::EnteredLeftGoal) {
	  player_two_score_keeper.add_point();
	}

	if (result == Ball::EnteredRightGoal) {
	  player_one_score_keeper.add_point();
	}
      }

      controller.update(ball.get_position(), playing_field);
    
      SDL_FillRect(display_screen, NULL, background_color);
      SDL_FillRect(display_screen, &playing_field, playing_field_color);
      ball.draw(display_screen);
      user_paddle.draw(display_screen);
      computer_paddle.draw(display_screen);
      player_one_score_keeper.draw(display_screen);
      player_two_score_keeper.draw(display_screen);
      break;

    default:
      // it shouldn't ever get here but if it does then just quit the game since it will loop forever otherwise
      exit_game_loop = true;
      break;
    }
    SDL_Flip(display_screen);

    while (SDL_GetTicks() < ticks_at_previous_frame + TicksPerFrame) {}
    ticks_at_previous_frame = SDL_GetTicks();
  }

  TTF_CloseFont(size_10_mono_font);

  return 0;
}
