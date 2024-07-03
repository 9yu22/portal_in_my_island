#pragma once

struct WorldLocation {
	float x, y, z;
};

class GameObject
{
public:
	WorldLocation pos;

public:
	GameObject();
	~GameObject();
	WorldLocation GetWorldLocation(WorldLocation pos) { return pos; }
	void SetWorldLocation(WorldLocation new_pos);
};

