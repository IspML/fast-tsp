#ifndef POKEMON_H
#define POKEMON_H

#include <iostream>
#include <string>

using namespace std;


enum RegionType
{
	LAND,
	SEA,
	COAST
};

struct Pokemon
{
	const int xPos;
	const int yPos;

	Pokemon(int x, int y) :
		xPos(x), yPos(y) { }

	inline RegionType region() const {
		if (xPos < 0 && yPos < 0) return SEA;
		if (xPos == 0 || yPos == 0) return COAST;
		
		return LAND;
	}
}; // class Pokemon


#endif // POKEMON_H