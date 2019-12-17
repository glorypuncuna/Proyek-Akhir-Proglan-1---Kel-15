#ifndef COLLISION_H
#define COLLISION_H

#include "status.h"

int collision2d(float x1, float y1, float x2, float y2, int w1, int h1, int w2, int h2);
void platformCollision(dnd *game);

#endif // COLLISION_H
