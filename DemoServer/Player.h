#pragma once
#include <array>
#include "GameObject.h"
#include "Item.h"
class Player : public GameObject
{
public:
	short id;
	std::array<Item, 1> player_items;

public:
	Player();
	~Player();
};

