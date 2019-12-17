#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "status.h"

void animateIdle(SDL_Renderer *renderer, dnd *game);
void animateWalk(SDL_Renderer *renderer, dnd *game);

#endif // ANIMATION_H
