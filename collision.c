#include <stdio.h>
#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "collision.h"
#include "game.h"
#include "render.h"
#include "status.h"

int collision2d(float x1, float y1, float x2, float y2, int w1, int h1, int w2, int h2)
{
    return (!((x1 > (x2+w2)) || (x2 > (x1+w1)) || (y1 > (y2+h2)) || (y2 > (y1+h1))));
}

void platformCollision(dnd *game)
{
    //Let's check out if our character actually hits the enemy, shall we?
    for(int i = 0; i < ENEMY_NUMBERS; i++)
    {
        if(collision2d(game->player.x, game->player.y, game->enemy[i].x, game->enemy[i].y, game->player.w, game->player.h, game->enemy[i].w, game->enemy[i].h))
        {
            if(!game->player.dead)
            {
                game->player.dead = 1;
                Mix_HaltChannel(game->channel);
                Mix_PlayChannel(-1, game->fail, 0);
            }
            break;
        }
    }

    //What if he falls to his demise?
    if(game->player.y > 480)
    {
        if(!game->player.dead)
        {
            game->player.dead = 1;
            Mix_HaltChannel(game->channel);
            Mix_PlayChannel(-1, game->fail, 0);
        }
    }

    //For brick collision detection
    for(int i = 0; i < GROUND_NUMBERS; i++)
    {
        if(game->player.x + game->player.w/2 > game->ground[i].x && game->player.x + game->player.w/2 < game->ground[i].x + game->ground[i].w)
        {
            //Check if character hits the bottom of the square
            if(game->player.y < game->ground[i].y + game->ground[i].h && game->player.y > game->ground[i].y && game->player.dy < 0)
            {
                //Correct y position
                game->player.y = game->ground[i].y + game->ground[i].h;

                //Since character hits the bottom of the square, he won't be moving
                game->player.dy = 0;
                game->player.onGround = 1;
            }
        }
        if(game->player.x + game->player.w > game->ground[i].x && game->player.x < game->ground[i].x + game->ground[i].w)
        {
            //Have our character land on top of the square?
            if(game->player.y + game->player.h > game->ground[i].y && game->player.y < game->ground[i].y && game->player.dy > 0)
            {
                //Correct y position
                game->player.y = game->ground[i].y - game->player.h;

                //Since he just landed, he won't be moving anytime soon
                game->player.dy = 0;
                if(!game->player.onGround)
                    game->player.onGround = 1;
            }
        }

        if(game->player.y + game->player.h > game->ground[i].y && game->player.y<game->ground[i].y + game->ground[i].h)
        {
            //Is our character bumping against the right side of the square?
            if(game->player.x < game->ground[i].x + game->ground[i].w && game->player.x + game->player.w > game->ground[i].x + game->ground[i].w && game->player.dx < 0)
            {
                //Correct x position
                game->player.x = game->ground[i].x + game->ground[i].w;

                //No more walking to the right for him
                game->player.dx = 0;
            }
            //Is our character bumping against the left side of the square now?
            else if(game->player.x + game->player.w > game->ground[i].x && game->player.x < game->ground[i].x && game->player.dx > 0)
            {
                //Correct x position
                game->player.x = game->ground[i].x - game->player.w;

                //Aaaand, no more walking to the left for him.
                game->player.dx = 0;
            }
        }
    }

}


