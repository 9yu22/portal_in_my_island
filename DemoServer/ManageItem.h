#pragma once
#include "GameObject.h"
#include <array>
#include <chrono>
#include <atomic>

enum Item_type { STONE, AMETHYST, RUBY, DIAMOND, NONE };

class Item : public GameObject
{
public:
	int item_id;
	Item_type item_type;

public:
	Item();
	Item(int id, Item_type type, int location_number);

	~Item();
};

class ItemSpawnManager
{
public:
	//std::array<Item, 4> manage_items;
	std::array<double, 4> item_spawn_time;
	std::array<double, 4> calculate_spawn_time;
	std::chrono::steady_clock::time_point lastUpdateTime;

	
	//Item_type spawn_item_type = NONE;

	std::atomic<int> calculateTimeForInt = 0;

	int count_id = 0;

public:
	ItemSpawnManager();
	~ItemSpawnManager();

	//void SpawnItem(Item_type type);
	void InitStartTime();
	void UpdateTimeForSpawn(double elapsedTime);
	Item_type CheckCanSpawn();
	void CalculateItemSpawnTime();
};