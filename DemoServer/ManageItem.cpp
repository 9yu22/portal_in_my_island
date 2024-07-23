#include "ManageItem.h"
#include <iostream>
#include <mutex>
#include <random>


Item::Item()
{
}

Item::~Item()
{
}





ManageItem::ManageItem()
{
	manage_items[ROCK].item_type = ROCK;
	manage_items[AMETHYST].item_type = AMETHYST;
	manage_items[RUBY].item_type = RUBY;
	manage_items[DIAMOND].item_type = DIAMOND;

	for (auto& i : manage_items) {
		switch (i.item_type) {
		case ROCK:
			i.item_spawn_time = 5.0;
			break;
		case AMETHYST:
			i.item_spawn_time = 10.0;
			break;
		case RUBY:
			i.item_spawn_time = 20.0;
			break;
		case DIAMOND:
			i.item_spawn_time = 40.0;
			break;
		}
	}
	lastUpdateTime = std::chrono::steady_clock::now();
	// 스위치 내부에 아이템 스폰 위치 아이템별로 지정 필요
}

ManageItem::~ManageItem()
{
}

void ManageItem::SpawnItem(Item_type type)
{
	switch (type) {
	case ROCK:
		manage_items[ROCK].is_spawned = true;
		manage_items[ROCK].calculate_spawn_time = 0.0;
		spawn_item_type = ROCK;
		break;

	case AMETHYST:
		manage_items[AMETHYST].is_spawned = true;
		manage_items[AMETHYST].calculate_spawn_time = 0.0;
		spawn_item_type = AMETHYST;
		break;

	case RUBY:
		manage_items[RUBY].is_spawned = true;
		manage_items[RUBY].calculate_spawn_time = 0.0;
		spawn_item_type = RUBY;
		break;

	case DIAMOND:
		manage_items[DIAMOND].is_spawned = true;
		manage_items[DIAMOND].calculate_spawn_time = 0.0;
		spawn_item_type = DIAMOND;
		break;

	default:
		spawn_item_type = NONE;
		break;
	}

}

void ManageItem::CheckCanSpawnItem(double elapsedTime)
{
	for (auto& i : manage_items) {
		if (false == i.is_spawned) // 스폰 되어있지 않다면 시간 계산
			i.calculate_spawn_time += elapsedTime;
		else
			continue;
	}

	for (auto& i : manage_items) {
		if (i.calculate_spawn_time >= i.item_spawn_time) {
			SpawnItem(static_cast<Item_type>(i.item_type));
			break;
		}

		else
			SpawnItem(static_cast<Item_type>(NONE));
	}
}

void ManageItem::CalculateItemSpawnTime()
{
	auto currentTime = std::chrono::steady_clock::now();

	// 지난 시간과 현재 시간의 차이 계산
	//std::chrono::duration<double> elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastUpdateTime);
	std::chrono::duration<double> elapsedTime = currentTime - lastUpdateTime;

	// 마지막 업데이트 시간을 현재 시간으로 갱신
	lastUpdateTime = currentTime;

	//std::cout << elapsedTime.count() << std::endl;
	CheckCanSpawnItem(elapsedTime.count());
}
