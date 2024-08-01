#pragma once
#include <array>
#include <chrono>
#include <atomic>
#include <mutex>
#include "Session.h"
#include "ManageItem.h"

enum ROOM_STATE { EMPTY, PLAY, LOBBY };

class Room
{
public:
	std::array<Session, 4> sessions;
	ItemSpawnManager item_manager;
	std::mutex room_mutex;
	/*std::atomic<int> in_game_timer = 0;
	std::atomic<int> last_time;*/
	int in_gmae_timer = 0;
	int last_time;
	int room_state = EMPTY;


public:
	Room();
	~Room();

	void StartGame();
	void UpdateTimer();
	void EndGame();
};

