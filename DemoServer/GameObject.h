#pragma once

struct WorldLocation {
	float x, y, z;
};

class GameObject
{
public:
	WorldLocation location;

public:
	GameObject();
	~GameObject();
	WorldLocation GetWorldLocation(WorldLocation location) { return location; }
	void SetWorldLocation(WorldLocation new_pos);
	void SetWorldLocation(float new_x, float new_y, float new_z);
};

