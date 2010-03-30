#ifndef SCORE_KEEPER_H
#define SCORE_KEEPER_H

#include "drawable.h"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

class ScoreKeeper : public IDrawable {
 public:
  ScoreKeeper();
  ScoreKeeper(TTF_Font* font, SDL_Rect destination);
  const int get_score();
  void add_point();
  bool has_enough_points_to_win();
  void reset_points();

  void draw(SDL_Surface* screen);

  static const int PointsToWin = 10;

 private:
  void init(TTF_Font* font, SDL_Rect destination);
  int score;
  TTF_Font* font;
  SDL_Rect destination;
};


#endif // SCORE_KEEPER_H
