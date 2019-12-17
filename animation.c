#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game.h"
#include "status.h"

void animateIdle(SDL_Renderer *renderer, dnd *game)
{
    int i = 0;
    SDL_Surface *surface;

    surface = IMG_Load("rogue.png");
    game->playerIdle = SDL_CreateTextureFromSurface(renderer, surface);

    while((game.player->x == game.player->dx) && (game.player->y == game.player->dy))
    {
        Uint32 time = (SDL_GetTicks()/1000)%10;


    }
}
