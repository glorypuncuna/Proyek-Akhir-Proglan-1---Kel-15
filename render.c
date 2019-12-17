#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "game.h"
#include "status.h"

void renderPlatform(dnd *game)
{
    SDL_RenderClear(game->renderer);

    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);

    //Make the background
    SDL_Rect bgRect = {0, 0, WIDTH, HEIGHT};
    SDL_RenderCopy(game->renderer, game->bg, NULL, &bgRect);

    //Draw the bricks
    for(int i = 0; i < GROUND_NUMBERS; i++)
    {
      SDL_Rect groundRect = {game->scroll + game->ground[i].x, game->ground[i].y, game->ground[i].w, game->ground[i].h};
      SDL_RenderCopy(game->renderer, game->bricks, NULL, &groundRect);
    }

    //Draw the enemies
    for(int i = 0; i < ENEMY_NUMBERS; i++)
    {
      SDL_Rect enemiesRect = {game->scroll + game->enemy[i].x, game->enemy[i].y, game->enemy[i].w, game->enemy[i].h};
      SDL_RenderCopy(game->renderer, game->enemies, NULL, &enemiesRect);
    }

    //Draw the player, or us
    SDL_Rect rectDouble = {game->player.x + game->player.dx + game->scroll, game->player.y + game->player.dy, game->player.h, game->player.w};
    SDL_RenderCopyEx(game->renderer, game->playerFrames[game->player.animationFrames], NULL, &rectDouble, 0, NULL, (game->player.left == 1));

    SDL_RenderPresent(game->renderer);
}

void startGame(dnd *game)
{
    char words[128] = "";

    sprintf(words, "x %d", (int)game->player.lives);

    //Clear the screen
    SDL_RenderClear(game->renderer);

    //Create color preset
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color red = { 202, 0, 42, 255 };

    //For showing how many lives left
    game->surface = TTF_RenderText_Blended(game->font, words, white);
    game->textW = game->surface->w;
    game->textH = game->surface->h;
    game->text = SDL_CreateTextureFromSurface(game->renderer, game->surface);
    SDL_FreeSurface(game->surface);

    //For showing objective
    game->surface = TTF_RenderText_Blended(game->font, "Get to the end alive!", red);
    game->anotherTextW = game->surface->w;
    game->anotherTextH = game->surface->h;
    game->anotherText = SDL_CreateTextureFromSurface(game->renderer, game->surface);
    SDL_FreeSurface(game->surface);

}

void renderStartGame(dnd *game)
{
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);

    //Clear the screen
    SDL_RenderClear(game->renderer);

    //Draw our character
    SDL_Rect playerRect = { 250, 216, 48, 48 };
    SDL_RenderCopyEx(game->renderer, game->playerFrames[0], NULL, &playerRect, 0, NULL, (game->player.left == 1));

    //Draw the lives remaining text
    SDL_Rect textRect = { 300, 240-game->textH/2, game->textW, game->textH };
    SDL_RenderCopy(game->renderer, game->text, NULL, &textRect);

    //And finally, draw the objective
    SDL_Rect anotherTextRect = { 320-game->anotherTextW/2, 100-game->anotherTextH/2, game->anotherTextW, game->anotherTextH };
    SDL_RenderCopy(game->renderer, game->anotherText, NULL, &anotherTextRect);

    //What happens if our ol' reliable guy dies?
    if(game->player.dead)
    {
      SDL_Rect deadRect = { game->scroll+game->player.x-24+38/2+10, game->player.y-24-83/2, 38, 38 };
      SDL_RenderCopyEx(game->renderer, game->explosion, NULL, &deadRect, 0, NULL, (game->time%20 < 10));
    }

}

void clearStart(dnd *game)
{
    SDL_DestroyTexture(game->text);
    game->text = NULL;

    SDL_DestroyTexture(game->anotherText);
    game->anotherText = NULL;
}

void startGameOver(dnd *game)
{
    //Create color preset
    SDL_Color white = { 255, 255, 255, 255 };

    //Well, let's create the text shall we?
    game->surface = TTF_RenderText_Blended(game->font, "Cue the GAME OVER SCREEN!", white);
    game->textW = game->surface->w;
    game->textH = game->surface->h;
    game->text = SDL_CreateTextureFromSurface(game->renderer, game->surface);
    SDL_FreeSurface(game->surface);
}

void renderGameOver(dnd *game)
{
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);

    //Usual stuff here, shouldn't be surprised by now. Clear the screen
    SDL_RenderClear(game->renderer);

    //Set the "canvas" to white now.
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);

    SDL_Rect textRect = { 320-game->textW/2, 240-game->textH/2, game->textW, game->textH };
    SDL_RenderCopy(game->renderer, game->text, NULL, &textRect);
}

void clearGameOver(dnd *game)
{
    SDL_DestroyTexture(game->text);
    game->text = NULL;
}

void startWin(dnd *game)
{
    //Another function, another identical color palette
    SDL_Color white = { 255, 255, 255, 255 };

    game->surface = TTF_RenderText_Blended(game->font, "You actually...win?!", white);
    game->textW = game->surface->w;
    game->textH = game->surface->h;
    game->text = SDL_CreateTextureFromSurface(game->renderer, game->surface);
    SDL_FreeSurface(game->surface);
}

void renderWin(dnd *game)
{
    //Clear the screen...yet once again
    SDL_RenderClear(game->renderer);

    //Set the canvas to white...
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);

    SDL_Rect textRect = { 320-game->textW/2, 240-game->textH/2, game->textW, game->textH };
    SDL_RenderCopy(game->renderer, game->text, NULL, &textRect);
}

void clearWin(dnd *game)
{
    SDL_DestroyTexture(game->text);
    game->text = NULL;
}
