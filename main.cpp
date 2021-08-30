#include "Game.h"

using namespace sf;

int main()
{
	//Init srand
	std::srand(static_cast<unsigned>(time(NULL)));

	//Init Game engine
	Game game;
	//game loop
	while (game.running() && !game.getEndGame()) {
		//Update
		game.update();
		//Render
		game.render();
	}
	//end of loop
	return 0;
}
