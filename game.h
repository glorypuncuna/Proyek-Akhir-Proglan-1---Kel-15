#ifndef GAME_H
#define GAME_H

#include "status.h"

int processEvent(dnd *game);
void userInput(dnd *game);
int loadGame(dnd *game);
void renderAll(dnd *game);
int initialSetup();

#endif
