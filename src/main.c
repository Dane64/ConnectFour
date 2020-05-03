#include "inputHandler.h"
#include "menu.h"
#include "game.h"

stGamePad stMcd;
tStGame stGame;

int main()
{
	startVisualizer();
	startInput();
	loadGame(stGame.auiHighScore);

	stGame.eState = MainMenu;
	stGame.eTurn = Yellow;
	stGame.usiLevel = 5;
	stGame.uiFieldWidth = 7;
	stGame.uiFieldHeight = 6;
	stGame.usiPlayerDisk = 0;
	stGame.usiOpponentDisk = 1;

//	splash();

	while (stGame.eState != Quit)
	{
		inputRead(&stMcd);
		updateVisualizer();

		if (stGame.eState >= Playing)
		{
			gameLoop(&stGame, &stMcd);
		}
		else if (stGame.eState <= MainMenu)
		{
			mainMenu(&stGame, &stMcd);
		}

		showNavigationButtons(&stGame);	
		closeVisualizer();
	}

	finishVisualizer();
	sceKernelExitProcess(0);
	return 0;
}