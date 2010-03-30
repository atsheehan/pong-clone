#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>

#include "drawable.h"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

class Menu : public IDrawable {
 public:
  Menu(const std::string& menu_title, const std::vector<std::string>& selections);
  Menu(const std::string& menu_title, const std::vector<std::string>& selections, int start_y, int center_x, TTF_Font* font);

  int get_current_selection();
  void next_selection();
  void previous_selection();
  void disable_selection(int selection_index);
  void enable_selection(int selection_index);

  void draw(SDL_Surface* screen);

  const static int InvalidSelection = -1;

 private:
  void init(const std::string& menu_title, const std::vector<std::string>& selections, int start_y, int center_x, TTF_Font* font);

  std::string title;
  std::vector<std::string> selections;
  int current_selection;
  std::vector<bool> disabled_list;
  int number_of_disabled_entries;
  int start_y;
  int center_x;
  TTF_Font* font;
  int line_spacing;
};

#endif // MENU_H
