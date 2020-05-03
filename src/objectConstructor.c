#include "objectConstructor.h"

void boardConstructor(tStGame *stGame)
{
    // Make gamefield equal size (better for drawing sprites)
	stGame->usiCellHeight = HEIGHT / (stGame->uiFieldHeight + 1);
	stGame->usiCellWidth = stGame->usiCellHeight;

    unsigned short uiIncreaseRow = 0;
	stGame->Field = malloc(sizeof(tStBoard *)*stGame->uiFieldHeight);

    for (int i=0; i<stGame->uiFieldHeight; i++)
    {
        uiIncreaseRow += stGame->usiCellHeight;
        stGame->Field[i] = malloc(sizeof(tStBoard)*stGame->uiFieldWidth);
        for (int j=0; j<stGame->uiFieldWidth; j++)
        {
            stGame->Field[i][j].eData = Empty;
            stGame->Field[i][j].uiY = uiIncreaseRow;
            stGame->Field[i][j].uiX = j == 0 ? stGame->usiCellWidth/2 : stGame->Field[i][j-1].uiX + stGame->usiCellWidth;
        }
    }
}

void boardDestructor(tStGame *stGame)
{
    for (int i=0; i<stGame->uiFieldHeight; i++) {
        free(stGame->Field[i]);
    }
    free(stGame->Field);
}