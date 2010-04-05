#include "text_renderer.h"
#include <iostream>

using std::string;

TextRenderer::TextRenderer() {
  init(NULL);
}

TextRenderer::TextRenderer(TTF_Font* font) {
  init(font);
}

TextRenderer::~TextRenderer() {
  free_text_surface();
}

void TextRenderer::draw(SDL_Surface* screen) {
  if (text_surface != NULL) {
    SDL_BlitSurface(text_surface, NULL, screen, &position);
  }
}

const SDL_Rect& TextRenderer::get_position() {
  return position;
}

const string& TextRenderer::get_text() {
  return text;
}

void TextRenderer::set_text(const string& new_text) {
  this->text = new_text;
  create_text_surface();
}

void TextRenderer::set_center_position(int center_x, int center_y) {
  this->center_x = center_x;
  this->center_y = center_y;

  calculate_position();
}

void TextRenderer::init(TTF_Font* font) {
  position.x = position.y = 0;
  position.w = position.h = 0;
  center_x = center_y = 0;
  text = "";
  font_color.r = font_color.g = font_color.b = 0;
  
  this->font = font;
  create_text_surface();
}

void TextRenderer::create_text_surface() {
  if (font != NULL) {
    free_text_surface();
    text_surface = TTF_RenderText_Solid(font, text.c_str(), font_color);
    calculate_position();
  }
}

void TextRenderer::free_text_surface() {
  if (text_surface != NULL) {
    SDL_FreeSurface(text_surface);
    text_surface = NULL;
  }
}

void TextRenderer::calculate_position() {
  if (text_surface != NULL) {
    position.w = text_surface->w;
    position.h = text_surface->h;
    position.x = center_x - (position.w / 2);
    position.y = center_y - (position.h / 2);
  }
}
