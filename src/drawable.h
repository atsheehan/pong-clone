#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "SDL/SDL.h"

class IDrawable {
 public:
  virtual ~IDrawable() {}
  virtual void draw(SDL_Surface* surface) = 0;
};

#endif // DRAWABLE_H
