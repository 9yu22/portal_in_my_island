#pragma once

struct WorldLocation {
	float x, y, z;
};

struct WorldRotation {
	float pitch, yaw, roll;
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

