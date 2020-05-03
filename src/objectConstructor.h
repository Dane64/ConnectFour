#ifndef _OBJECTCONSTRUCTOR_H_
#define _OBJECTCONSTRUCTOR_H_

#include "inputHandler.h"
#include <stdlib.h>
#include <math.h>

typedef enum tEnumState{
	BoardSettings,
	LevelSettings,
	DiskSettings,
	MainMenu,
	Playing,
	Paused,
	Credits,
	Quit
} tEnumState;

typedef enum tEnumPlayer{
	Empty = 0,
	Red = 1,
	Yellow = -1
} tEnumPlayer;

typedef struct tStBoard
{
	unsigned short uiX;
	unsigned short uiY;
	tEnumPlayer eData;
} tStBoard;

typedef struct tStGame
{
	tStBoard **Field;
	tEnumState eState;
	tEnumPlayer eTurn;
	unsigned char usiPlayerDisk;
	unsigned char usiOpponentDisk;
	unsigned char usiLevel;
	unsigned char usiCellWidth;
	unsigned char usiCellHeight;
	unsigned short uiFieldWidth;
	unsigned short uiFieldHeight;
	unsigned short auiHighScore[2];
} tStGame;

typedef struct Move
{
	signed short iVal;
	unsigned char usiRow;
	unsigned char usiCol;
	tEnumPlayer ePlayer;
} Move;

#if (PLATFORM == Vita)
      #define WIDTH 960
      #define HEIGHT 544
#elif (PLATFORM == New3DS)
      #define WIDTH 800
      #define HEIGHT 400
#else
      #define WIDTH 0
      #define HEIGHT 0
#endif

void boardConstructor(tStGame *stGame);
void boardDestructor(tStGame *stGame);

#endif