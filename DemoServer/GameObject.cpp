#include "GameObject.h"

GameObject::GameObject()
{
	location.x = 0.f;
	location.y = 0.f;
	location.z = 0.f;
}

GameObject::~GameObject()
{
}

void GameObject::SetWorldLocation(WorldLocation new_location)
{
	location.x = new_location.x;
	location.y = new_location.y;
	location.z = new_location.z;
}

void GameObject::SetWorldLocation(float new_x, float new_y, float new_z)
{
	location.x = new_x;
	location.y = new_y;
	location.z = new_z;
}
