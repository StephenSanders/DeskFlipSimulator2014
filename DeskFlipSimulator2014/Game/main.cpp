#include <Engine/Defines.h>
#include <Engine/Debug.h>
#include <Engine/Game.h>
#include <Engine/Sound.h>

#include "gameScene.h"

int main(int argc, char *argv[])
{
	Debug("Hello, world!\n");
	int restart = 0;
	Game *game = new Game();
	game->SetScene(new GameScene());
	game->Main();
	restart = ((GameScene *)game->GetScene())->restart;
	delete game;
	while (restart){
		restart = 0;
		game = new Game();
		game->SetScene(new GameScene());
		game->Main();
		restart = ((GameScene *)game->GetScene())->restart;
		delete game;
	}
	return 0;
}
