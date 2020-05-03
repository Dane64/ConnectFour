#include "objectVisualizer.h"

#define BLACK   RGBA8(  0,   0,   0, 255)
#define GREY    RGBA8(111, 111, 111, 255)
#define WHITE   RGBA8(255, 255, 255, 255)
#define RED     RGBA8(255,   0,   0, 255)
#define ORANGE  RGBA8(255, 165,   0, 255)
#define YELLOW  RGBA8(255, 255,   0, 255)
#define GREEN   RGBA8(  0, 255,   0, 255)
#define CYAN    RGBA8(  0, 255, 255, 255)
#define BLUE    RGBA8(  0,   0, 255, 255)
#define INDIGO  RGBA8(111,   0, 255, 255)
#define MAGENTA RGBA8(255,   0, 255, 255)

#define BROWN   RGBA8(139,  69,  19, 255)
#define WHEAT   RGBA8(245, 222, 179, 255)
#define PINK    RGBA8(255, 192, 203, 255)
#define KHAKI   RGBA8(240, 230, 140, 255)

typedef struct tStCutout
{
	unsigned short uiWidth;
	unsigned short uiHeigth;
	float rHorScale;
	float rVertScale;
} tStCutout;

vita2d_font *font;
// vita2d_pgf *font = NULL; 
vita2d_texture *C4 = NULL;
SceDateTime Time;
tStCutout stPieceCut;
tStCutout stPadCut;
tStCutout stBarCut;

const float rSpeed = 0.5; // Disk falling speed pixels per ms
unsigned short uiCentre;
unsigned short uiMiddle;

char sText[MENUBOXES][15] =
{"PLAY",
 "LEVEL",
 "BOARD",
 "DISK SELECT",
 "QUIT GAME"};

char sLevel[LEVELBOXES][15] =
{"VERY EASY",
 "EASY",
 "NORMAL",
 "HARD"};
 //"VERY HARD"};

void startVisualizer()
{
    // vita2d_init();
	vita2d_init_advanced_with_msaa(1048576, SCE_GXM_MULTISAMPLE_4X);
	font = vita2d_load_font_file("app0:Font.otf");
	// font = vita2d_load_custom_pgf("app0:Font.pgf");
	C4 = vita2d_load_PNG_file("app0:C4.png");
	vita2d_set_vblank_wait(1);
	vita2d_set_clear_color(GREY);

	stPieceCut.uiWidth = 100;
	stPieceCut.uiHeigth = 100;

	stPadCut.uiWidth = 100;
	stPadCut.uiHeigth = 100;

	stBarCut.uiWidth = 300;
	stBarCut.uiHeigth = 100;
}

void updateVisualizer()
{
	vita2d_start_drawing();
	vita2d_clear_screen();
}

void drawBackground(tStGame *stGame)
{
	vita2d_draw_rectangle(stGame->Field[0][0].uiX, stGame->Field[0][0].uiY, stGame->usiCellWidth*stGame->uiFieldWidth, stGame->usiCellHeight*stGame->uiFieldHeight, WHITE);
}

void drawBoard(tStGame *stGame)
{
	stPieceCut.rHorScale = (float)stGame->usiCellWidth/stPieceCut.uiWidth;
	stPieceCut.rVertScale = (float)stGame->usiCellHeight/stPieceCut.uiHeigth;

	for (unsigned char i = 0; i < stGame->uiFieldHeight; i++)
	{
		for (unsigned char j = 0; j < stGame->uiFieldWidth; j++)
		{
			//vita2d_draw_fill_circle(stGame->Field[i][j].uiX+(stGame->usiCellWidth/2), stGame->Field[i][j].uiY+(stGame->usiCellHeight/2), stGame->usiCellHeight*45/100, WHITE);
			if (stGame->Field[i][j].eData == Red)
			{
				vita2d_draw_texture_part_scale(C4, stGame->Field[i][j].uiX, stGame->Field[i][j].uiY, stGame->usiOpponentDisk*stPieceCut.uiWidth, 0, stPieceCut.uiWidth, stPieceCut.uiHeigth, stPieceCut.rHorScale, stPieceCut.rVertScale);
				//vita2d_draw_fill_circle(stGame->Field[i][j].uiX+(stGame->usiCellWidth/2), stGame->Field[i][j].uiY+(stGame->usiCellHeight/2), stGame->usiCellHeight*45/100, RED);
			}
			else if (stGame->Field[i][j].eData == Yellow)
			{
				vita2d_draw_texture_part_scale(C4, stGame->Field[i][j].uiX, stGame->Field[i][j].uiY, stGame->usiPlayerDisk*stPieceCut.uiWidth, 0, stPieceCut.uiWidth, stPieceCut.uiHeigth, stPieceCut.rHorScale, stPieceCut.rVertScale);
				//vita2d_draw_fill_circle(stGame->Field[i][j].uiX+(stGame->usiCellWidth/2), stGame->Field[i][j].uiY+(stGame->usiCellHeight/2), stGame->usiCellHeight*45/100, YELLOW);
			}
			vita2d_draw_texture_part_scale(C4, stGame->Field[i][j].uiX, stGame->Field[i][j].uiY, 600, 0, stPieceCut.uiWidth, stPieceCut.uiHeigth, stPieceCut.rHorScale, stPieceCut.rVertScale);
		}
	}
}

bool drawDiskAnimation(tStGame *stGame, Move *eMove, bool xDropDisk, unsigned char usiDiskPos)
{
	bool xDiskDropped;
	static unsigned int tOld;
	static float s;
	unsigned int tDelta;
	sceRtcGetCurrentClockLocalTime(&Time);
	unsigned int tNew = sceRtcGetMicrosecond(&Time);

	if (tNew > tOld)
	{
		tDelta = (tNew - tOld) / 1000;
	}
	else
	{
		tDelta = (tNew + (tOld - abs((tNew-tOld)))) / 1000;
	}

	if (eMove->ePlayer == Yellow)
	{
		vita2d_draw_texture_part_scale(C4, stGame->Field[0][usiDiskPos].uiX, s, stGame->usiPlayerDisk*stPieceCut.uiWidth, 0, stPieceCut.uiWidth, stPieceCut.uiHeigth, stPieceCut.rHorScale, stPieceCut.rVertScale); // Make sure row is fixed to zero availablePosition could return -1
		//vita2d_draw_fill_circle(stGame->Field[0][j].uiX+(stGame->usiCellWidth/2), (stGame->usiCellHeight/2), stGame->usiCellHeight*45/100, YELLOW);
	}
	else
	{
		vita2d_draw_texture_part_scale(C4, stGame->Field[0][eMove->usiCol].uiX, s, stGame->usiOpponentDisk*stPieceCut.uiWidth, 0, stPieceCut.uiWidth, stPieceCut.uiHeigth, stPieceCut.rHorScale, stPieceCut.rVertScale); // Make sure row is fixed to zero availablePosition could return -1
		//vita2d_draw_fill_circle(stGame->Field[0][j].uiX+(stGame->usiCellWidth/2), (stGame->usiCellHeight/2), stGame->usiCellHeight*45/100, RED);
	}

	if (xDropDisk)
	{	s += rSpeed * min(tDelta, 40); // Cap at 40 to ensure nice animation at higher difficulty (this will take significantly longer to calculate)
		xDiskDropped = false;
	}
	else if (s > 0 && s < (float)stGame->Field[eMove->usiRow][eMove->usiCol].uiY)
	{
		s += rSpeed * tDelta;
		xDiskDropped = false;
	}
	else if (s >= (float)stGame->Field[eMove->usiRow][eMove->usiCol].uiY)
	{
		s = 0;
		stGame->Field[eMove->usiRow][eMove->usiCol].eData = eMove->ePlayer;
		xDiskDropped = true;
	}
	else
	{
		xDiskDropped = true;
	}
	

//	vita2d_font_draw_textf(font, 600, 250, GREEN, 50, "t %i",tDelta);
	tOld = tNew;
	return xDiskDropped;
}

void showGameTitle()
{
	int iTextWidth;
	int iTextHeight;

//	vita2d_pgf_text_dimensions(font, 1.0f, "CONNECT FOUR", &iTextWidth, &iTextHeight);
	vita2d_font_text_dimensions(font, 50, "CONNECT FOUR", &iTextWidth, &iTextHeight);

	uiCentre = WIDTH/2;
	uiMiddle = iTextHeight + 10;

	// vita2d_pgf_draw_textf(font, 10, 20, ORANGE, 1.0f, "CONNECT FOUR");
	vita2d_font_draw_textf(font, uiCentre-iTextWidth/2, uiMiddle, ORANGE, 50, "CONNECT FOUR");
}

void drawMainMenu()
{
	int iTextWidth;
	int iTextHeight;

	stBarCut.rHorScale = 1;
	stBarCut.rVertScale = 1;

	uiCentre = WIDTH/2;
	uiMiddle = HEIGHT/(MENUBOXES + 1);
	usiNrOfBoxesOnCurrentPage = MENUBOXES;

	float rHorPos = uiCentre-((float)stBarCut.uiWidth*stBarCut.rHorScale/2);
	float rVertPos = (float)stBarCut.uiHeigth*stBarCut.rVertScale/2;

	for (unsigned char i = 1; i < MENUBOXES+1; i++)
	{
		vita2d_draw_texture_part_scale(C4, rHorPos , uiMiddle*i, 400, 200, stBarCut.uiWidth, stBarCut.uiHeigth, stBarCut.rHorScale, stBarCut.rVertScale);
		vita2d_font_text_dimensions(font, 25, sText[i-1], &iTextWidth, &iTextHeight);
		vita2d_font_draw_textf(font, uiCentre-(iTextWidth/2), (uiMiddle*i)+(rVertPos+(iTextHeight/2)), ORANGE, 25, sText[i-1]); // half of the texture width - half of the text width centers the text
	}
}

void drawLevelSelector()
{
	int iTextWidth;
	int iTextHeight;

	stBarCut.rHorScale = 1;
	stBarCut.rVertScale = 1;

	uiCentre = WIDTH/2;
	uiMiddle = HEIGHT/(LEVELBOXES + 1);
	usiNrOfBoxesOnCurrentPage = LEVELBOXES;

	float rHorPos = uiCentre-((float)stBarCut.uiWidth*stBarCut.rHorScale/2);
	float rVertPos = (float)stBarCut.uiHeigth*stBarCut.rVertScale/2;

	for (unsigned char i = 1; i < LEVELBOXES+1; i++)
	{
		vita2d_draw_texture_part_scale(C4, rHorPos , uiMiddle*i, 400, 200, stBarCut.uiWidth, stBarCut.uiHeigth, stBarCut.rHorScale, stBarCut.rVertScale);
		vita2d_font_text_dimensions(font, 25, sLevel[i-1], &iTextWidth, &iTextHeight);
		vita2d_font_draw_textf(font, uiCentre-(iTextWidth/2), (uiMiddle*i)+(rVertPos+(iTextHeight/2)), ORANGE, 25, sLevel[i-1]); // half of the texture width - half of the text width centers the text
	}
}

void showSelectedDisk(tStGame *stGame)
{
	uiCentre = WIDTH/3;
	uiMiddle = HEIGHT/4;

	vita2d_font_draw_textf(font, uiCentre*1, uiMiddle*1+stPieceCut.uiHeigth/2, ORANGE, 30, "PLAYER :");
	vita2d_draw_texture_part(C4, uiCentre*2 , uiMiddle*1, stGame->usiPlayerDisk*stPieceCut.uiWidth, 0, stPieceCut.uiWidth, stPieceCut.uiHeigth);

	vita2d_font_draw_textf(font, uiCentre*1, uiMiddle*2+stPieceCut.uiHeigth/2, ORANGE, 30, "OPPONENT :");
	vita2d_draw_texture_part(C4, uiCentre*2 , uiMiddle*2, stGame->usiOpponentDisk*stPieceCut.uiWidth, 0, stPieceCut.uiWidth, stPieceCut.uiHeigth);

	if (stGame->eTurn == Yellow)
	{
		vita2d_draw_texture_part(C4, uiCentre*1 , uiMiddle*3, stGame->usiPlayerDisk*stPieceCut.uiWidth, 0, stPieceCut.uiWidth, stPieceCut.uiHeigth);
	}
	else if (stGame->eTurn == Red)
	{
		vita2d_draw_texture_part(C4, uiCentre*1 , uiMiddle*3, stGame->usiOpponentDisk*stPieceCut.uiWidth, 0, stPieceCut.uiWidth, stPieceCut.uiHeigth);
	}
	
	vita2d_font_draw_textf(font, uiCentre*2-(uiCentre/2), uiMiddle*3+stPieceCut.uiHeigth/2, ORANGE, 30, "WILL BEGIN");	
}

void showDifficulty(tStGame *stGame)
{
	int iTextWidth;
	int iTextHeight;
	unsigned char i;

	for (i = 1; i < stGame->usiLevel+1; i++)
	{
		vita2d_draw_texture_part_scale(C4, WIDTH-(10*i), 20, stGame->usiOpponentDisk*stPieceCut.uiWidth, 0, stPieceCut.uiWidth, stPieceCut.uiHeigth, 10/(float)stPieceCut.uiWidth, 10/(float)stPieceCut.uiHeigth);
	}

	vita2d_font_text_dimensions(font, 15, "Selected Level", &iTextWidth, &iTextHeight);
	vita2d_font_draw_textf(font, WIDTH-(10*i)-iTextWidth, 30, ORANGE, 15, "Selected Level");
}

void showBoardSize(tStGame *stGame)
{
	int iTextWidth;
	int iTextHeight;

	vita2d_font_text_dimensions(font, 15, "Board size is 9 x 9", &iTextWidth, &iTextHeight);
	vita2d_font_draw_textf(font, WIDTH-iTextWidth, 50, ORANGE, 15, "Board size is %i x %i",stGame->uiFieldWidth, stGame->uiFieldHeight);
}

void showHighScore(tStGame *stGame)
{
	int iTextWidth;
	int iTextHeight;

	vita2d_font_text_dimensions(font, 15, "Player won xxx times", &iTextWidth, &iTextHeight);
	vita2d_font_draw_textf(font, WIDTH-iTextWidth, 75, ORANGE, 15, "Player won %03d times", stGame->auiHighScore[0]);
	vita2d_font_text_dimensions(font, 15, "Computer won xxx times", &iTextWidth, &iTextHeight);
	vita2d_font_draw_textf(font, WIDTH-iTextWidth, 100, ORANGE, 15, "Computer won %03d times", stGame->auiHighScore[1]);
}

void showNavigationButtons(tStGame *stGame)
{
	stPadCut.rHorScale = 0.4;
	stPadCut.rVertScale = 0.4;

	unsigned char i = 1;
	unsigned char uiOffset = 10;
	float rVertPos = (float)stPadCut.uiHeigth*stPadCut.rVertScale+uiOffset;

	if (stGame->eState == MainMenu)
	{
		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 100, 200, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Move Down");
		i++;

		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 0, 200, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Move Up");
		i++;

		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 0, 100, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Select");	
	}
	else if (stGame->eState == LevelSettings)
	{
		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 100, 200, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Move Down");
		i++;

		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 0, 200, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Move Up");
		i++;

		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 0, 100, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Select");
		i++;

		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 200, 100, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Return");				
	}
	else if (stGame->eState == BoardSettings)
	{
		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 200, 200, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Shrink X");
		i++;

		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 300, 200, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Grow X");
		i++;

		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 100, 200, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Shrink Y");
		i++;

		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 0, 200, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Grow Y");
		i++;

		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 200, 100, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Return");
	}
	else if (stGame->eState == DiskSettings)
	{
		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 200, 200, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Choose Disk");
		i++;

		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 300, 200, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Choose Disk");
		i++;

		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 300, 100, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Who starts");
		i++;

		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 200, 100, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Return");
	}
	else if (stGame->eState == Playing)
	{
		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 200, 200, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Move Left");
		i++;

		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 300, 200, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Move Right");
		i++;

		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 0, 100, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Drop Disk");
		i++;

		vita2d_draw_texture_part_scale(C4, WIDTH-150, HEIGHT-(i*rVertPos), 200, 100, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
		vita2d_font_draw_textf(font, WIDTH-150+(stPadCut.uiWidth*stPadCut.rHorScale+uiOffset), HEIGHT-(i*rVertPos)+(stPadCut.uiHeigth*stPadCut.rVertScale/2), ORANGE, 15, "Return");
	}
}

void showMenuArrow(unsigned char usiPos)
{
	stPadCut.rHorScale = 0.5;
	stPadCut.rVertScale = 0.5;
	float rVertPos = ((float)stBarCut.uiHeigth*stBarCut.rVertScale/2)-((float)stPadCut.uiHeigth*stPadCut.rVertScale/2);

	float rHorPos = uiCentre-((float)stBarCut.uiWidth*stBarCut.rHorScale/2)-((float)stPadCut.uiWidth*stPadCut.rHorScale);
	vita2d_draw_texture_part_scale(C4, rHorPos, uiMiddle*usiPos+rVertPos, 200, 200, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);

	rHorPos = uiCentre+((float)stBarCut.uiWidth*stBarCut.rHorScale/2);
	vita2d_draw_texture_part_scale(C4, rHorPos, uiMiddle*usiPos+rVertPos, 300, 200, stPadCut.uiWidth, stPadCut.uiHeigth, stPadCut.rHorScale, stPadCut.rVertScale);
}

void closeVisualizer()
{
    vita2d_wait_rendering_done();
	vita2d_end_drawing();
	vita2d_swap_buffers();
	sceDisplayWaitVblankStart();
}

void finishVisualizer()
{
	vita2d_free_texture(C4);
	vita2d_free_font(font);
    vita2d_fini();
}