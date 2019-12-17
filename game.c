#include <stdio.h>
#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "collision.h"
#include "render.h"
#include "status.h"

int loadGame(dnd *game)
{
//--------------------Load up platforming related image----------------------
    //Background
    game->surface = IMG_Load("images/Map.png");
    if(game->surface == NULL)
    {
        printf("Hey, i can't find Map.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    //Create background texture
    game->bg = SDL_CreateTextureFromSurface(game->renderer, game->surface);
    SDL_FreeSurface(game->surface);

    //Ground
    game->surface = IMG_Load("images/platform.png");
    if(game->surface == NULL)
    {
        printf("Whoa, where is platform.png...........*vanished*\n\n");
        SDL_Quit();
        exit(1);
    }
    //Create ground texture
    game->bricks = SDL_CreateTextureFromSurface(game->renderer, game->surface);
    SDL_FreeSurface(game->surface);

    //Enemy
    game->surface = IMG_Load("images/unknown.png");
    if(game->surface == NULL)
    {
        printf("Report, sir! No unknown to be found here!\n\n");
        SDL_Quit();
        exit(1);
    }
    //Create ground texture
    game->enemies = SDL_CreateTextureFromSurface(game->renderer, game->surface);
    SDL_FreeSurface(game->surface);
//---------------------------------------------------------------------------

//------------------------Load up player animation-------------------------------------
    //Frame 1
    game->surface = IMG_Load("images/playerFrame1.png");
    if(game->surface == NULL)
    {
        printf("Where is playerFrame1.png again...?\n\n");
        SDL_Quit();
        exit(1);
    }
    game->playerFrames[0] = SDL_CreateTextureFromSurface(game->renderer, game->surface);
    SDL_FreeSurface(game->surface);

    //Frame 2
    game->surface = IMG_Load("images/playerFrame2.png");
    if(game->surface == NULL)
    {
        printf("Excuse me, where can i find playerFrame2.png?\n\n");
        SDL_Quit();
        exit(1);
    }
    game->playerFrames[1] = SDL_CreateTextureFromSurface(game->renderer, game->surface);
    SDL_FreeSurface(game->surface);

    //Frame 3
    game->surface = IMG_Load("images/playerFrame3.png");
    if(game->surface == NULL)
    {
        printf("I don't see anything named playerFrame3.png 'ere.\n\n");
        SDL_Quit();
        exit(1);
    }
    game->playerFrames[2] = SDL_CreateTextureFromSurface(game->renderer, game->surface);
    SDL_FreeSurface(game->surface);

    //Dead frame
    game->surface = IMG_Load("images/explode.png");
    if(game->surface == NULL)
    {
        printf("BOOOOOOOM!!!!...wait, what do you mean we don't have explode.png? Somebody, grab it!");
        SDL_Quit();
        exit(1);
    }
    game->explosion = SDL_CreateTextureFromSurface(game->renderer, game->surface);
    SDL_FreeSurface(game->surface);
//---------------------------------------------------------------------------------------

//---------------------------------Load up font------------------------------------------------
    //Load font
    game->font = TTF_OpenFont("font/pixel.ttf", 32);
    if(!game->font)
    {
        printf("Another obscure file here, can't find pixel.ttf anywhere through the coast.");
        SDL_Quit();
        exit(1);
    }
//---------------------------------------------------------------------------------------------

//--------------------Initialize music-------------------------
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

    //Jumping sound effect
    game->jump = Mix_LoadWAV("music/jump.wav");
    if(game->jump == NULL)
    {
        printf("Jumpman? JumpMAAAN! ...Well, i guess there's only fallman now.");
        SDL_Quit();
        exit(1);
    }

    //Death sound effect
    game->fail = Mix_LoadWAV("music/fail.wav");
    if(game->fail == NULL)
    {
        printf("No failure here, capiche?");
        SDL_Quit();
        exit(1);
    }

    //Music for map
    game->music = Mix_LoadWAV("music/map.wav");
    Mix_PlayChannel(-1, game->music, -1);
    if (game->music == NULL)
    {
        printf("What the...what music are you talking about?");
        SDL_Quit();
        exit(1);
    }
//-------------------------------------------------------------

//---------Initialize various player parameters----------
    game->player.x = 100;
    game->player.y = 200;
    game->player.dx = 0;
    game->player.dy = 0;
    game->player.w = 32;
    game->player.h = 32;
    game->player.lives = 3;
    game->player.dead = 0;
    game->player.onGround = 0;
    game->player.slowDown = 0;
    game->player.left = 0;
    game->player.animationFrames = 0;
    game->player.parameter = ALIVE;
    game->player.demise = -1;

    startGame(game);

    game->time = 0;
    game->scroll = 0;
//-------------------------------------------------------

//-----------------Initialize grounds--------------------
    for(int i = 0; i < GROUND_NUMBERS - 30; i++)
    {
        game->ground[i].w = 256;
        game->ground[i].h = 64;
        game->ground[i].x = i*384;
        if(i == 0)
            game->ground[i].y = 400;
        else
            game->ground[i].y = 400-rand()%200;
    }

    for(int i = GROUND_NUMBERS - 30; i < GROUND_NUMBERS; i++)
    {
        game->ground[i].w = 256;
        game->ground[i].h = 64;
        game->ground[i].x = 350 + rand()%25600;
        if(i % 3 == 0)
            game->ground[i].y = 200;
        else
            game->ground[i].y = 350;
    }
//------------------------------------------------------

//----------------Initialize enemies--------------------
    for(int i = 0; i < ENEMY_NUMBERS; i++)
    {
        game->enemy[i].tempx = 320+rand()%25600;
        game->enemy[i].tempy = rand()%480;
        game->enemy[i].w = 48;
        game->enemy[i].h = 48;
        game->enemy[i].mode = rand()%2 + 1;
        game->enemy[i].phase = 2*3.14*(rand()%360)/360.0f;
    }
//------------------------------------------------------
}

void process(dnd *game)
{
//----Used for various things, including animation-----
    game->time++;
//-----------------------------------------------------

//---------------------Checking whether the player is alive-------------------
    if(game->player.parameter == ALIVE)
    {
        if(game->time > 150)
        {
            clearStart(game);
            game->player.parameter = START_GAME;
        }
    }

    else if(game->player.parameter == GAME_OVER)
    {
        if(game->time > 210)
        {
            SDL_Quit();
            exit(0);
        }
    }

    else if(game->player.parameter == START_GAME)
    {
        if(!game->player.dead)
        {
            //For various parameters involving our great character's movements
            game->player.x += game->player.dx;
            game->player.y += game->player.dy;
            game->player.dy += GRAVITY;

            if(game->player.x != 0 && game->player.onGround && !game->player.slowDown)
            {
                if(game->time % 10 == 0)
                {
                    if(game->player.animationFrames == 0)
                        game->player.animationFrames = 1;
                    else if(game->player.animationFrames == 1)
                        game->player.animationFrames = 2;
                    else
                        game->player.animationFrames = 0;
                }
            }


            if(game->player.x > 25600)
            {
                startWin(game);
                game->player.parameter = WIN;
            }

            //Time for the enemy to move!
            for(int i = 0; i < ENEMY_NUMBERS; i++)
            {
                game->enemy[i].x = game->enemy[i].tempx;
                game->enemy[i].y = game->enemy[i].tempy;
                if(game->enemy[i].mode == 1)
                    game->enemy[i].x = game->enemy[i].tempx+(float)sin((double)(game->enemy[i].phase+game->time*0.04f))*75;
                else
                    game->enemy[i].y = game->enemy[i].tempy+cos((double)(game->enemy[i].phase+game->time*0.04f))*75;
            }
        }

        if(game->player.dead && game->player.demise < 0)
            game->player.demise = 150;
        if(game->player.demise >= 0)
            {
                game->player.demise--;
                if(game->player.demise < 0)
                {
                    game->player.lives--;

                    if(game->player.lives >= 0)
                    {
                        startGame(game);
                        game->player.parameter = ALIVE;
                        game->time = 0;

                        //reset
                        game->player.dead = 0;
                        game->player.x = 100;
                        game->player.y = 200;
                        game->player.dx = 0;
                        game->player.dy = 0;
                        game->player.onGround = 0;
                        for(int i = 0; i < ENEMY_NUMBERS; i++)
                        {
                            game->enemy[i].tempx = 320+rand()%25600;
                            game->enemy[i].tempy = rand()%480;
                            game->enemy[i].w = 48;
                            game->enemy[i].h = 48;
                            game->enemy[i].mode = rand()%2 + 1;
                            game->enemy[i].phase = 2*3.14*(rand()%360)/360.0f;
                        }
                    }

                    else
                    {
                        startGameOver(game);
                        game->player.parameter = GAME_OVER;
                        game->time = 0;
                    }
                }
            }
    }

    game->scroll = -game->player.x+320;
    if(game->scroll > 0)
        game->scroll = 0;
    if(game->scroll < -25920)
        game->scroll = -25920;
//----------------------------------------------------------------------------

}

int userInput(dnd *game)
{
    SDL_Event input;
    int end = 0;

//----------------Check user inputs (dubbed as event)--------
    while(SDL_PollEvent(&input))
    {
        switch(input.type)
        {
            case SDL_WINDOWEVENT_CLOSE:
            {
                if(game->window)
                {
                    SDL_DestroyWindow(game->window);
                    game->window = NULL;
                    end = 1;
                }
            }
            break;
            case SDL_KEYDOWN:
            {
                switch(input.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        end = 1;
                    break;
                    case (SDL_KEYUP || SDLK_z):
                        if(game->player.onGround)
                        {
                            game->player.dy = -8;
                            game->player.onGround = 0;
                            Mix_PlayChannel(-1, game->jump, 0);
                        }
                    break;
                    case (SDLK_x):
                        if(game->player.left == 1)
                            game->player.dx += -12;
                        if(game->player.left == 0)
                            game->player.dx += 12;
                    break;
                }
            }
            break;
            case SDL_QUIT:
                end = 1;
                break;

        }
    }

//----------------------------------------------------------

//----------------Getting keyboard input to walk------------

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if(game->player.onGround && (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_Z]))
    {
        game->player.dy = -8;
        game->player.onGround = 0;
        Mix_PlayChannel(-1, game->jump, 0);
    }

    if(state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_Z])
    {
        game->player.dy -= 0.21f;
    }

    else if(state[SDL_SCANCODE_LEFT])
    {
        game->player.dx -= 0.4;
        if(game->player.dx < -6)
            game->player.dx = -6;
        game->player.left = 1;
        game->player.slowDown = 0;
    }

    else if(state[SDL_SCANCODE_RIGHT])
    {
        game->player.dx += 0.4;
        if(game->player.dx > 6)
            game->player.dx = 6;
        game->player.left = 0;
        game->player.slowDown = 0;
    }

    else if(state[SDL_SCANCODE_X])
    {
        if(game->player.left == 0)
        {
            game->player.dx += -0.6;
            if(game->player.dx < -12)
                game->player.dx = 12;
        }

        if(game->player.left == 1)
        {
             game->player.dx += 0.6;
             if(game->player.dx > 12)
                game->player.dx = 12;
        }

    }

    else
    {
        game->player.animationFrames = 0;
        game->player.dx *= 0.8f;
        game->player.slowDown = 1;
    }
//-------------------------------------------------
    return end;
}

void renderAll(dnd *game)
{
    renderPlatform(game);
}

int initalSetup()
{
//-------------------Initialize SDL-------------------
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_ShowCursor(0);
    int end = 0;

    dnd game;
//----------------------------------------------------

//---------------------------------------Create a Window-------------------------------------------------------
    game.window = SDL_CreateWindow
    (
        "DnD",
        SDL_WINDOWPOS_UNDEFINED, //X position
        SDL_WINDOWPOS_UNDEFINED, //Y position
        WIDTH,
        HEIGHT,
        0
    );
    if (game.window == NULL)
    {

        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//-------------------------------------------------------------------------------------------------------------

    //Initialize font
    TTF_Init();

//---------------------Loading up game-----------------------------
    loadGame(&game);

    while(!end)
    {
        if(userInput(&game) == 1)
            end = 1;
        process(&game);
        platformCollision(&game);
        renderAll(&game);

    }
//-----------------------------------------------------------------

//--------------------Shutting down SDL------------------------

//===============Unload memories used for textures=============
    SDL_DestroyTexture(game.bg);
    for(int i = 0; i < 3; i++)
    {
        SDL_DestroyTexture(game.playerFrames[i]);
    }
//=============================================================

//=================Close up various SDL functions==============
    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);

    SDL_FreeWAV(game.jump);
    SDL_FreeWAV(game.fail);
    SDL_FreeWAV(game.music);
    Mix_CloseAudio();

    TTF_CloseFont(game.font);
    TTF_Quit();

    SDL_Quit();
//=============================================================

//-------------------------------------------------------------
    return 0;
}
