#include "score_keeper.h"
#include <sstream>

ScoreKeeper::ScoreKeeper() {
  SDL_Rect default_position;
  default_position.x = default_position.y = 0;
  default_position.w = default_position.h = 20;
  init(NULL, default_position);
}

ScoreKeeper::ScoreKeeper(TTF_Font* font, SDL_Rect destination) {
  init(font, destination);
}

void ScoreKeeper::init(TTF_Font* font, SDL_Rect destination) {
  this->font = font;
  this->destination = destination;
  score = 0;
}

void ScoreKeeper::add_point() {
  ++score;
}

bool ScoreKeeper::has_enough_points_to_win() {
  return score >= PointsToWin ? true : false;
}

void ScoreKeeper::draw(SDL_Surface* screen) {
  if (!font) {
    return;
  }

  std::string score_string;
  std::stringstream out;
  out << score;
  score_string = out.str();

  SDL_Color black = {0, 0, 0};
  SDL_Surface* score_surface = TTF_RenderText_Solid(font, score_string.c_str(), black);
  SDL_BlitSurface(score_surface, NULL, screen, &destination);
  SDL_FreeSurface(score_surface);
}

const int ScoreKeeper::get_score() {
  return score;
}
