#include "Player.h"

player::player()
{
	x = 900.f;
	y = 1600.f;
	z = 1000.f;
}

player::~player()
{
}

void player::UpdateLocation(float newX, float newY, float newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}
