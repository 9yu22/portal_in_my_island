#pragma once
#include <array>
#include "GameObject.h"
#include "Item.h"
#include "Portal.h"

class Player : public GameObject
{
public:
	int m_id;
	float m_hp;
	WorldRotation m_rotation;
	std::array<Item, 1> m_player_items;
	Portal portal;

public:
	Player();
	~Player();

	WorldRotation GetWorldRotation();
	void SetWorldRotation(WorldRotation new_rotation);
	void SetWorldRotation(float new_pitch, float new_yaw, float new_roll);
};

