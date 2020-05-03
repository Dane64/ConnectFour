#include "game.h"

Move eMove;
Move eMoveOld;

const signed short MAX = 30000; 
const signed short MIN = -30000; 

unsigned char USICOL;
unsigned char USIROW;

bool xInit = true;
bool xDiskDropped = true;
unsigned char j;

void switchPlayer(tStGame *stGame)
{
	if (stGame->eTurn == Red)
		stGame->eTurn = Yellow;
	else
		stGame->eTurn = Red;
}

bool movesLeft(tStGame *stGame)
{
	for (unsigned char i = 0; i < stGame->uiFieldHeight; i++)
		for (unsigned char j = 0; j < stGame->uiFieldWidth; j++)
			if (stGame->Field[i][j].eData == Empty)
				return true;
	
    return false;
}

signed char availablePosition(tStGame *stGame, unsigned char usiSelCol)
{
	signed char siFreeSpot = stGame->uiFieldHeight - 1;

	for (unsigned char i = 0; i < stGame->uiFieldHeight; i++)
		if (stGame->Field[i][usiSelCol].eData != Empty)
		{
			siFreeSpot = i - 1;
			break;
		}

	return siFreeSpot;
}

bool checkWinner(tStGame *stGame, tEnumPlayer ePlayer)
{
	for (unsigned char i = 0; i < stGame->uiFieldHeight; i++)
		for (unsigned char j = 0; j < stGame->uiFieldWidth - 3; j++)
			if (stGame->Field[i][j].eData == ePlayer && stGame->Field[i][j].eData == stGame->Field[i][j+1].eData && stGame->Field[i][j].eData == stGame->Field[i][j+2].eData && stGame->Field[i][j].eData == stGame->Field[i][j+3].eData)
				return true;

	for (unsigned char j = 0; j < stGame->uiFieldWidth; j++)
		for (unsigned char i = 0; i < stGame->uiFieldHeight - 3; i++)
            if (stGame->Field[i][j].eData == ePlayer && stGame->Field[i][j].eData == stGame->Field[i+1][j].eData && stGame->Field[i][j].eData == stGame->Field[i+2][j].eData && stGame->Field[i][j].eData == stGame->Field[i+3][j].eData)
				return true;

	for (unsigned char i = 0; i < stGame->uiFieldHeight - 3; i++)
		for (unsigned char j = 0; j < stGame->uiFieldWidth - 3; j++)
            if (stGame->Field[i][j].eData == ePlayer && stGame->Field[i][j].eData == stGame->Field[i+1][j+1].eData && stGame->Field[i][j].eData == stGame->Field[i+2][j+2].eData && stGame->Field[i][j].eData == stGame->Field[i+3][j+3].eData)
				return true;

	for (unsigned char i = 0; i < stGame->uiFieldHeight - 3; i++)
		for (unsigned char j = 3; j < stGame->uiFieldWidth; j++)
            if (stGame->Field[i][j].eData == ePlayer && stGame->Field[i][j].eData == stGame->Field[i+1][j-1].eData && stGame->Field[i][j].eData == stGame->Field[i+2][j-2].eData && stGame->Field[i][j].eData == stGame->Field[i+3][j-3].eData)
				return true;

	return false;
}

signed short checkScore(tStGame *stGame)
{
	signed short iScore = 0;
	unsigned char usiCounter = 0;

	// Score extra points for center positions
	//  0  1  2  3  4  5  6
	// [ ][ ][ ][x][ ][ ][ ] 0
	// [ ][ ][ ][x][ ][ ][ ] 1
	// [ ][ ][ ][x][ ][ ][ ] 2
	// [ ][ ][ ][x][ ][ ][ ] 3
	// [ ][ ][ ][x][ ][ ][ ] 4
	// [ ][ ][ ][x][ ][ ][ ] 5
	for (unsigned char i = 0; i < stGame->uiFieldHeight; i++)
	{
		if (stGame->Field[i][stGame->uiFieldWidth/2].eData == stGame->eTurn)
		{
			usiCounter++;
		}
	}

	iScore += usiCounter * 3;
	usiCounter = 1;

	// Score horizontal
	//  0  1  2  3  4  5  6
	// [ ][ ][ ][ ][ ][ ][ ] 0
	// [ ][ ][ ][ ][ ][ ][ ] 1
	// [ ][ ][x][x][x][x][ ] 2
	// [ ][ ][ ][ ][ ][ ][ ] 3
	// [ ][ ][ ][ ][ ][ ][ ] 4
	// [ ][ ][ ][ ][ ][ ][ ] 5
	for (unsigned char i = 0; i < stGame->uiFieldHeight; i++)
	{
		for (unsigned char j = 0; j < stGame->uiFieldWidth - 1; j++)
		{
			if (stGame->Field[i][j].eData == !Empty && stGame->Field[i][j].eData == stGame->Field[i][j+1].eData) // There are two identical chips
			{
				usiCounter++;
				iScore += (stGame->Field[i][j].eData == stGame->eTurn) ? pow(usiCounter, usiCounter) : -pow(2, usiCounter); // if chips belong to opponent subtract from score
			}
			else
			{
				usiCounter = 1;
			}		
		}
	}

	usiCounter = 1;

	// Score vertical
	//  0  1  2  3  4  5  6
	// [ ][ ][ ][ ][ ][ ][ ] 0
	// [ ][ ][ ][ ][ ][ ][ ] 1
	// [ ][ ][ ][x][ ][ ][ ] 2
	// [ ][ ][ ][x][ ][ ][ ] 3
	// [ ][ ][ ][x][ ][ ][ ] 4
	// [ ][ ][ ][x][ ][ ][ ] 5
	for (unsigned char j = 0; j < stGame->uiFieldWidth; j++)
	{
		for (unsigned char i = 0; i < stGame->uiFieldHeight - 1; i++)
		{
			if (stGame->Field[i][j].eData == !Empty && stGame->Field[i][j].eData == stGame->Field[i+1][j].eData) // There are two identical chips
			{
				usiCounter++;
				iScore += (stGame->Field[i][j].eData == stGame->eTurn) ? pow(usiCounter, usiCounter) : -pow(2, usiCounter); // if chips belong to opponent subtract from score
			}
			else
			{
				usiCounter = 1;
			}
		}
	}

	usiCounter = 1;

	// Score negative diagonal
	//  0  1  2  3  4  5  6
	// [x][ ][ ][ ][ ][ ][ ] 0
	// [ ][x][ ][ ][ ][ ][ ] 1
	// [ ][ ][x][ ][ ][ ][ ] 2
	// [ ][ ][ ][x][ ][ ][ ] 3
	// [ ][ ][ ][ ][ ][ ][ ] 4
	// [ ][ ][ ][ ][ ][ ][ ] 5
	for (unsigned char i = 0; i < stGame->uiFieldHeight - 2; i++)
	{
		for (unsigned char j = 0; j < stGame->uiFieldWidth - 2; j++)
		{
			if (stGame->Field[i][j].eData == !Empty && stGame->Field[i][j].eData == stGame->Field[i+1][j+1].eData) // There are two identical chips
			{
				usiCounter++;
                if (stGame->Field[i][j].eData == stGame->Field[i+2][j+2].eData) // There are three identical chips
                {
                    usiCounter++;
                }
                
				iScore += (stGame->Field[i][j].eData == stGame->eTurn) ? pow(usiCounter, usiCounter) : -pow(2, usiCounter); // if chips belong to opponent subtract from score
				usiCounter = 1;
			}
		}
	}

	usiCounter = 1;

	// Score positive diagonal
	//  0  1  2  3  4  5  6
	// [ ][ ][ ][x][ ][ ][ ] 0
	// [ ][ ][x][ ][ ][ ][ ] 1
	// [ ][x][ ][ ][ ][ ][ ] 2
	// [x][ ][ ][ ][ ][ ][ ] 3
	// [ ][ ][ ][ ][ ][ ][ ] 4
	// [ ][ ][ ][ ][ ][ ][ ] 5
	for (unsigned char i = 0; i < stGame->uiFieldHeight - 2; i++)
	{
		for (unsigned char j = 2; j < stGame->uiFieldWidth; j++)
		{
			if (stGame->Field[i][j].eData == !Empty && stGame->Field[i][j].eData == stGame->Field[i+1][j-1].eData) // There are two identical chips
			{
				usiCounter++;
                if (stGame->Field[i][j].eData == stGame->Field[i+2][j-2].eData) // There are three identical chips
                {
                    usiCounter++;
                }
                
				iScore += (stGame->Field[i][j].eData == stGame->eTurn) ? pow(usiCounter, usiCounter) : -pow(2, usiCounter); // if chips belong to opponent subtract from score
				usiCounter = 1;
			}
		}
	}
	
	return iScore * stGame->eTurn;
}

signed short minimax(tStGame *stGame, unsigned char usiDepth, bool xMaximizing, signed short iAlpha, signed short iBeta)
{
	bool xTerminate = checkWinner(stGame, Red) || checkWinner(stGame, Yellow) || movesLeft(stGame) == false;

	if (xTerminate || usiDepth == 0)
	{
		if (xTerminate)
		{
			if (checkWinner(stGame, Red))
			{
				return MAX+usiDepth-25; // To ensure faster victory (25 is because depth will not exceed 25)
			}
			else if (checkWinner(stGame, Yellow))
			{
				return MIN-usiDepth+25;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return checkScore(stGame);
		}
	}

    if (xMaximizing) 
    { 
        signed short best = MIN; 
        for (unsigned char j = 0; j < stGame->uiFieldWidth; j++) 
        {
            signed char i = availablePosition(stGame, j);
			if (i >= 0)
			{
				stGame->Field[i][j].eData = Red;
				signed short val = minimax(stGame, usiDepth - 1, !xMaximizing, iAlpha, iBeta);
				stGame->Field[i][j].eData = Empty;
				if (val > best)
				{
					best = val;
					if (usiDepth == stGame->usiLevel)
					{
						USICOL = j;
						USIROW = i;
					}
				}

				iAlpha = max(iAlpha, best);
			}
  
            // Alpha Beta Pruning 
            if (iBeta <= iAlpha)
			{
                break;
			}
        }
        return best; 
    } 
    else
    { 
        signed short best = MAX;
        for (unsigned char j = 0; j < stGame->uiFieldWidth; j++) 
        {
            signed char i = availablePosition(stGame, j);
			if (i >= 0)
			{
				stGame->Field[i][j].eData = Yellow;
				signed short val = minimax(stGame, usiDepth - 1, !xMaximizing, iAlpha, iBeta);
				stGame->Field[i][j].eData = Empty; 
				if (val < best)
				{
					best = val;
					if (usiDepth == stGame->usiLevel)
					{
						USICOL = j;
						USIROW = i;
					}
				}

				iBeta = min(iBeta, best);
			}
  
            // Alpha Beta Pruning 
            if (iBeta <= iAlpha)
			{
                break;
			}
        }
        return best;
    } 
}

Move bestMove(tStGame *stGame)
{
	Move eMove;
	bool xMaximizing;

	if (stGame->eTurn == Red)
	{
		xMaximizing = true;
	}
	else
	{
		xMaximizing = false;
	}
	
	signed short val = minimax(stGame, stGame->usiLevel, xMaximizing, MIN, MAX);
	eMove.usiRow = USIROW;
	eMove.usiCol = USICOL;
	eMove.iVal = val;
	eMove.ePlayer = stGame->eTurn;

	return eMove;
}

void gameLoop(tStGame *stGame, stGamePad *stMcd)
{
	if (xInit)
	{
		boardConstructor(stGame);
		xInit = false;
	}

	bool xWinner = false;	
	drawBackground(stGame);
	if (stGame->eTurn == Red && xDiskDropped)
	{
		eMove = bestMove(stGame);
		switchPlayer(stGame);
	}
	else if (stGame->eTurn == Yellow && xDiskDropped)
	{
		eMove.ePlayer = stGame->eTurn;
		j = selectColumn(stGame, stMcd);
		signed char i = availablePosition(stGame, j);
		if (stMcd->stButt[1].xTrigger && i >= 0)
		{
			eMove.usiRow = i; ;eMove.usiCol = j;
			switchPlayer(stGame);
		}
	}
	xDiskDropped = drawDiskAnimation(stGame, &eMove, (eMove.usiRow != eMoveOld.usiRow || eMove.usiCol != eMoveOld.usiCol), j);
	drawBoard(stGame);
	showDifficulty(stGame);
	eMoveOld = eMove;

	if (xDiskDropped)
	{
		xWinner = checkWinner(stGame, eMove.ePlayer);
	}
	

	if (xWinner || movesLeft(stGame) == false || (stMcd->stButt[2].xTrigger && xDiskDropped)) // Game is finished / stopped by player
	{
		if (xWinner && eMove.ePlayer == Red)
		{
			stGame->auiHighScore[1]++;
			saveGame(stGame->auiHighScore);
		}
		else if (xWinner && eMove.ePlayer == Yellow)
		{
			stGame->auiHighScore[0]++;
			saveGame(stGame->auiHighScore);
		}

		xInit = true;
		boardDestructor(stGame);
		stGame->eState = MainMenu;
	}
}