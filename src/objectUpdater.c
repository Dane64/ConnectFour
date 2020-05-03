#include "objectUpdater.h"
#include "game.h"

unsigned char selectColumn(tStGame *stGame, stGamePad *stMcd)
{
    static unsigned char j;

    if (stMcd->stDpad[0].xLeft && stMcd->stDpad[0].xTrigger)
    {
        j--;
    }
    else if (stMcd->stDpad[0].xRight && stMcd->stDpad[0].xTrigger)
    {
        j++;
        if (j == stGame->uiFieldWidth)
        {
            j = 0;
        } 
    }
    
    j = max(0, min(j, stGame->uiFieldWidth - 1)); // Limits j between 0 and max of playing field (zero based playing field)

    return j;
}

bool loadGame(unsigned short *uiHighScore)
{
    SceUID fd = sceIoOpen("ux0:data/Connect4.save", SCE_O_RDONLY , 0777);
    if (fd < 0)
    {
        return 0;
    }

    sceIoRead(fd, uiHighScore, 4);
    sceIoClose(fd);

    return 1;
}

bool saveGame(unsigned short *uiHighScore)
{
    SceUID fd = sceIoOpen("ux0:data/Connect4.save", SCE_O_TRUNC | SCE_O_WRONLY | SCE_O_CREAT, 0777);
    if (fd < 0)
    {
        return 0;
    }

    sceIoWrite(fd, uiHighScore, 4);
    sceIoClose(fd);

    return 1;   
}