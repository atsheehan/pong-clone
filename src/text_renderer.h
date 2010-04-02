#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <string>
#include "SDL/SDL.h"
#include "drawable.h"

class TextRenderer : public IDrawable {
 public:
  TextRenderer();

  void draw(SDL_Surface* screen);
  const SDL_Rect& get_position();
  const std::string& get_text();

 private:
  void init();

  SDL_Rect position;
  std::string text;

};

#endif // TEXT_RENDERER_H
