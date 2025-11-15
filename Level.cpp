

#include "Level.h"


Level::Level()
{

}

void Level::render() {
	for (Sector& sector : sectors) {
		sector.draw();
	}
}


void Level::update() {

}