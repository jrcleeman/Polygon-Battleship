#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
	//Create game object
	Game g("config.txt");
	g.run(); //Run game
}