#ifndef _GAME_H_
#define _GAME_H_

#include "objectConstructor.h"
#include "objectUpdater.h"
#include "objectVisualizer.h"

#define max(a, b) ( ( a > b) ? a : b )
#define min(a, b) ( ( a < b) ? a : b )

bool movesLeft(tStGame *stGame);
void switchPlayer(tStGame *stGame);
signed char availablePosition(tStGame *stGame, unsigned char usiSelCol);
void gameLoop(tStGame *stGame, stGamePad *stMcd);

#endif