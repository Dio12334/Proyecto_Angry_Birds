#include "include/Game.h"

int main(){
	Game game;
	if(game.initialize()){
		game.runloop();
	}
	game.shutdown();
	return 0;
}
