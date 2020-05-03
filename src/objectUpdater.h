#ifndef _OBJECTUPDATER_H_
#define _OBJECTUPDATER_H_

#include "objectConstructor.h"

unsigned char selectColumn(tStGame *stGame, stGamePad *stMcd);
bool loadGame(unsigned short *uiHighScore);
bool saveGame(unsigned short *uiHighScore);

#endif