#include "GameObject.h"

GameObject::GameObject()
{
	pos.x = 0.f;
	pos.y = 0.f;
	pos.z = 0.f;
}

GameObject::~GameObject()
{
}

void GameObject::SetWorldLocation(WorldLocation new_pos)
{
	pos.x = new_pos.x;
	pos.y = new_pos.y;
	pos.z = new_pos.z;
}
