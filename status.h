#ifndef STATUS_H
#define STATUS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define WIDTH 640
#define HEIGHT 480
#define ENEMY_NUMBERS 100
#define GROUND_NUMBERS 100
#define ALIVE 1
#define START_GAME 2
#define GAME_OVER 3
#define WIN 4
#define GRAVITY 0.35f

typedef struct
{
    //Basic attributes, x and y are coordinates
    float x, y;
    float dx, dy;
    int w, h;
    int lives;
    int parameter;
    int onGround, dead, left, slowDown;
    int demise;

    int animationFrames;
} Person;

typedef struct
{
    float x, y;
    float tempx, tempy;
    int w, h;
    int mode;
    float phase;
} Enemy;

typedef struct
{
    float x, y;
    int w, h;
} Ground;

typedef struct
{
    Person player;
    Enemy enemy[ENEMY_NUMBERS];
    Ground ground[GROUND_NUMBERS];

    int time;
    float scroll;

    //Surface
    SDL_Surface *surface;

//-------------------Variety of textures-----------------
    //Background
    SDL_Texture *bg;

    //Player animation texture
    SDL_Texture *playerFrames[3];

    //Enemy texture
    SDL_Texture *enemies;

    //Ground texture
    SDL_Texture *bricks;

    //Explode texture(when the player dies)
    SDL_Texture *explosion;

    //Text
    SDL_Texture *text;
    int textW, textH;

    SDL_Texture *anotherText;
    int anotherTextW, anotherTextH;
//-------------------------------------------------------

    //Load window and renderer
    SDL_Window *window;
    SDL_Renderer *renderer;

    //Load font
    TTF_Font *font;

//------------------Variety of music---------------------
    //Load up music
    int channel;
    Mix_Music *music;

    //Load up sound effects
    Mix_Chunk *jump, *fail;   //
//-------------------------------------------------------

} dnd;

#endif
