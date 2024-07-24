#pragma once
#include "GameObject.h"
#include <array>
#include <chrono>

class Item : public GameObject
{
public:
	//int item_id = -1;
	char item_type = -1;
	double item_spawn_time = 0.0;
	double calculate_spawn_time = -10.0;
	bool is_spawned = false;

public:
	Item();
	~Item();
};



enum Item_type { STONE, AMETHYST, RUBY, DIAMOND, NONE };

class ManageItem
{
public:
	std::array<Item, 4> manage_items;
	std::chrono::steady_clock::time_point lastUpdateTime;
	Item_type spawn_item_type = NONE;

public:
	ManageItem();
	~ManageItem();

	void SpawnItem(Item_type type);
	void CheckCanSpawnItem(double elapsedTime);
	void CalculateItemSpawnTime();
};