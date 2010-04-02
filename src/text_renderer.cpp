#include "text_renderer.h"

using std::string;

TextRenderer::TextRenderer() {
  init();
}

void TextRenderer::draw(SDL_Surface* screen) {

}

const SDL_Rect& TextRenderer::get_position() {
  return position;
}

const string& TextRenderer::get_text() {
  return text;
}

void TextRenderer::init() {
  position.x = position.y = 0;
  position.w = position.h = 0;
  text = "";
}
