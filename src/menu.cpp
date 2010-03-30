#include "menu.h"

#include <iostream>

using std::string;
using std::vector;

Menu::Menu(const string& menu_title, const vector<string>& selections) {
  init(menu_title, selections, 0, 0, NULL);
}

Menu::Menu(const string& menu_title, const vector<string>& selections, int start_y, int center_x, TTF_Font* font) {
  init(menu_title, selections, start_y, center_x, font);
}

void Menu::init(const string& menu_title, const vector<string>& selections, int start_y, int center_x, TTF_Font* font) {
  this->title = menu_title;
  this->selections = selections;
  if (this->selections.empty()) {
    current_selection = InvalidSelection;
  } else {
    current_selection = 0;
  }

  // create an equal sized vector of boolean values to determine which entries in the
  // list are disabled. initially they are all enabled so it should be a vector full of
  // false values.
  vector<string>::const_iterator iterator;
  for (iterator = this->selections.begin(); iterator != this->selections.end(); ++iterator) {
    disabled_list.push_back(false);
  }
  number_of_disabled_entries = 0;

  this->start_y = start_y;
  this->center_x = center_x;
  this->font = font;
  this->line_spacing = 0;
  if (this->font != NULL) {
    this->line_spacing = TTF_FontHeight(this->font) / 2;
  } 
}

int Menu::get_current_selection() {
  return current_selection;
}

void Menu::next_selection() {
  if (selections.size() - number_of_disabled_entries <= 0) {
    current_selection = InvalidSelection;
    return;
  }
  
  ++current_selection;
  if (current_selection >= selections.size()) {
    current_selection = 0;
  }

  if (disabled_list[current_selection]) {
    next_selection();
  }
}

void Menu::previous_selection() {
  if (selections.size() - number_of_disabled_entries <= 0) {
    current_selection = InvalidSelection;
    return;
  }

  --current_selection;
  if (current_selection < 0) {
    current_selection = selections.size() - 1;
  }

  if (disabled_list[current_selection]) {
    previous_selection();
  }
}

void Menu::disable_selection(int selection_index) {
  // check if the index is valid, and then check to make sure it hasn't previously
  // been disabled. if disabling the current selection, make sure the current selection
  // is moved to the next available option.

  if (selection_index >= 0 && selection_index < disabled_list.size()) {
    if (disabled_list[selection_index] != true) {
      disabled_list[selection_index] = true;
      ++number_of_disabled_entries;

      if (current_selection == selection_index) {
	next_selection();
      }
    }
  }
}

void Menu::enable_selection(int selection_index) {
  if (selection_index >= 0 && selection_index < disabled_list.size()) {
    if (disabled_list[selection_index] != false) {
      disabled_list[selection_index] = false;
      --number_of_disabled_entries;

      if (current_selection == InvalidSelection) {
	current_selection = selection_index;
      }
    }
  }
}

void Menu::draw(SDL_Surface* screen) {
  if (!font) {
    return;
  }
  SDL_Color normal = {0, 0, 0};
  SDL_Color disabled = {40, 40, 40};
  SDL_Color current_fg = {255, 255, 255};
  SDL_Color current_bg = {0, 0, 0};
  SDL_Color font_color = normal;
  SDL_Rect destination;
  destination.y = start_y;
  destination.x = destination.h = destination.w = 0; // default values

  SDL_Surface* text_surface;
  text_surface = TTF_RenderText_Solid(font, title.c_str(), font_color);
  destination.h = text_surface->h;
  destination.w = text_surface->w;
  destination.x = center_x - (destination.w / 2);
  
  SDL_BlitSurface(text_surface, NULL, screen, &destination);
  SDL_FreeSurface(text_surface);

  destination.y += 4 * line_spacing;

  for (int i = 0; i < selections.size(); ++i) {
    if (disabled_list[i] == true) {
      font_color = disabled;
    } else {
      font_color = normal;
    }
    
    if (current_selection == i) {
      text_surface = TTF_RenderText_Shaded(font, selections[i].c_str(), current_fg, current_bg);
    } else {
      text_surface = TTF_RenderText_Solid(font, selections[i].c_str(), font_color);
    }
    destination.h = text_surface->h;
    destination.w = text_surface->w;
    destination.x = center_x - (destination.w / 2);

    SDL_BlitSurface(text_surface, NULL, screen, &destination);
    SDL_FreeSurface(text_surface);
    destination.y += 3 * line_spacing;
  }
}
