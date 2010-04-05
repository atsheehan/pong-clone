#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "drawable.h"

class TextRenderer : public IDrawable {
 public:
  TextRenderer();
  TextRenderer(TTF_Font* font);
  ~TextRenderer();

  void draw(SDL_Surface* screen);
  const SDL_Rect& get_position();
  const std::string& get_text();

  void set_center_position(int center_x, int center_y);
  void set_text(const std::string& new_text);

 private:
  void init(TTF_Font* font);
  void create_text_surface();
  void free_text_surface();
  void calculate_position();

  SDL_Rect position;
  int center_x;
  int center_y;
  std::string text;
  TTF_Font* font;
  SDL_Surface* text_surface;
  SDL_Color font_color;
};

#endif // TEXT_RENDERER_H
