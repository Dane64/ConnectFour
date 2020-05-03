#include "menu.h"

void selectBoardSize(tStGame *stGame, stGamePad *stMcd)
{
    if (stMcd->stDpad[0].xUp && stMcd->stDpad[0].xTrigger)
        stGame->uiFieldHeight++;
    else if (stMcd->stDpad[0].xDown && stMcd->stDpad[0].xTrigger)
    {
        stGame->uiFieldHeight--;
    }

    if (stMcd->stDpad[0].xRight && stMcd->stDpad[0].xTrigger)
        stGame->uiFieldWidth++;
    else if (stMcd->stDpad[0].xLeft && stMcd->stDpad[0].xTrigger)
    {
        stGame->uiFieldWidth--;
    }
    
    stGame->uiFieldHeight = max(4, min(stGame->uiFieldHeight, 8)); // Limits height between 4 and 8
    stGame->uiFieldWidth = max(4, min(stGame->uiFieldWidth, 9)); // Limits width between 4 and 9

}

void selectLevel(tStGame *stGame, stGamePad *stMcd)
{
    if (stMcd->stDpad[0].xLeft && stMcd->stDpad[0].xTrigger && !stMcd->stButt[1].xHold)
    {
        stGame->usiLevel--;
        if (stGame->usiLevel == 0)
        {
            stGame->usiLevel = 9;
        }
    }
    else if (stMcd->stDpad[0].xRight && stMcd->stDpad[0].xTrigger && !stMcd->stButt[1].xHold)
    {
        stGame->usiLevel++;
        if (stGame->usiLevel == 9)
        {
            stGame->usiLevel = 0;
        } 
    }
    
    stGame->usiLevel = max(1, min(stGame->usiLevel, 8)); // Limits level between 1 and 8
}

unsigned char buttonSelector(tStGame *stGame, stGamePad *stMcd)
{
    static unsigned char usiPos;

    if (stMcd->stDpad[0].xUp && stMcd->stDpad[0].xTrigger && !stMcd->stButt[1].xHold)
    {
        usiPos--;
        if (usiPos == 0)
        {
            usiPos = usiNrOfBoxesOnCurrentPage;
        }
    }
    else if (stMcd->stDpad[0].xDown && stMcd->stDpad[0].xTrigger && !stMcd->stButt[1].xHold)
    {
        usiPos++;
        if (usiPos > usiNrOfBoxesOnCurrentPage)
        {
            usiPos = 0;
        }
    }

    usiPos = max(1, min(usiPos, usiNrOfBoxesOnCurrentPage)); // Limits arrow between 0 and number of boxes

    return usiPos;
}

void diskSelector(tStGame *stGame, stGamePad *stMcd)
{
    if (stMcd->stDpad[0].xLeft && stMcd->stDpad[0].xTrigger)
    {
        stGame->usiPlayerDisk--;
        if (stGame->usiPlayerDisk > 6)
        {
            stGame->usiPlayerDisk = 6;
        }
    }
    else if (stMcd->stDpad[0].xRight && stMcd->stDpad[0].xTrigger)
    {
        stGame->usiPlayerDisk++;
        if (stGame->usiPlayerDisk >= 6)
        {
            stGame->usiPlayerDisk = 0;
        } 
    }

    if (stGame->usiPlayerDisk == stGame->usiOpponentDisk)
    {
        stGame->usiOpponentDisk++;
        if (stGame->usiOpponentDisk >= 6)
        {
            stGame->usiOpponentDisk = 0;
        } 
    }
    

    stGame->usiPlayerDisk = max(0, min(stGame->usiPlayerDisk, 5)); // Limits disk between 0 and 5
    stGame->usiOpponentDisk = max(0, min(stGame->usiOpponentDisk, 5)); // Limits disk between 0 and 5
}

void mainMenu(tStGame *stGame, stGamePad *stMcd)
{
    showGameTitle();

    unsigned char usiSelectedButton = buttonSelector(stGame, stMcd);

    if (stMcd->stButt[2].xTrigger)
    {
        stGame->eState = MainMenu;
    }

    if (stGame->eState == MainMenu)
    {
        drawMainMenu();
        showMenuArrow(usiSelectedButton);

        if (usiSelectedButton == 1 && stMcd->stButt[1].xTrigger)
        {
            stGame->eState = Playing;
        }
        else if (usiSelectedButton == 2 && stMcd->stButt[1].xTrigger)
        {
            stGame->eState = LevelSettings;
        }
        else if (usiSelectedButton == 3 && stMcd->stButt[1].xTrigger)
        {
            stGame->eState = BoardSettings;
        }
        else if (usiSelectedButton == 4 && stMcd->stButt[1].xTrigger)
        {
            stGame->eState = DiskSettings;
        }        
        else if (usiSelectedButton == 5 && stMcd->stButt[1].xTrigger)
        {
            stGame->eState = Quit;
        }
    }
    else if (stGame->eState == LevelSettings)
    {
        // selectLevel(stGame, stMcd);
        drawLevelSelector();
        showMenuArrow(usiSelectedButton);

        if (stMcd->stButt[1].xTrigger)
        {
            stGame->usiLevel = usiSelectedButton*2;
        }   
    }
    else if (stGame->eState == BoardSettings)
    {

        selectBoardSize(stGame, stMcd);
        boardConstructor(stGame); // TODO: Find better method to show new playing field instead of constant creating and destroying

        drawBackground(stGame);
        drawBoard(stGame);

        boardDestructor(stGame);
    }
    else if (stGame->eState == DiskSettings)
    {
        if (stMcd->stButt[3].xTrigger)
        {
            switchPlayer(stGame);
        }

        diskSelector(stGame, stMcd);
        showSelectedDisk(stGame);
    }

    showBoardSize(stGame);
    showHighScore(stGame);
    showDifficulty(stGame);
}