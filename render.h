#ifndef GAME_H
#define GAME_H

#include "status.h"

void renderPlatform(dnd *game);
void startGame(dnd *game);
void renderStartGame(dnd *game);
void clearStart(dnd *game);
void startGameOver(dnd *game);
void renderGameOver(dnd *game);
void clearGameOver(dnd *game);
void startWin(dnd *game);
void renderWin(dnd *game);
void clearWin(dnd *game);

#endif
