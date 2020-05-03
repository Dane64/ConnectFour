#ifndef _OBJECTVISUALIZER_H_
#define _OBJECTVISUALIZER_H_

#include <vitasdk.h>
#include <vita2d.h>
#include <psp2/display.h>
#include <psp2/rtc.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
#include "objectConstructor.h"
#include "game.h"

#define MENUBOXES 5
#define LEVELBOXES 4

unsigned char usiNrOfBoxesOnCurrentPage;

void startVisualizer();
void updateVisualizer();
void drawBackground(tStGame *stGame);
void drawBoard(tStGame *stGame);
bool drawDiskAnimation(tStGame *stGame, Move *eMove, bool xDropDisk, unsigned char usiDiskPos);
void drawMainMenu();
void drawLevelSelector();
void showGameTitle();
void showSelectedDisk(tStGame *stGame);
void showButtonOptions(tStGame *stGame);
void showDifficulty(tStGame *stGame);
void showHighScore(tStGame * stGame);
void showBoardSize(tStGame *stGame);
void showNavigationButtons(tStGame *stGame);
void showMenuArrow(unsigned char usiPos);
void closeVisualizer();
void finishVisualizer();

#endif