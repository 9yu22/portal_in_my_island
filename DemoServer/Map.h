#pragma once
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include "protocol.h"

constexpr int BIG_CHUNK_WIDTH = 3;
constexpr int BIG_CHUNK_SIZE = BIG_CHUNK_WIDTH * BIG_CHUNK_WIDTH * BIG_CHUNK_WIDTH;
constexpr int SMALL_CHUNK_WIDTH = 64;
constexpr int SMALL_CHUNK_SIZE = SMALL_CHUNK_WIDTH * SMALL_CHUNK_WIDTH * SMALL_CHUNK_WIDTH;
constexpr int MAX_ARRAY_SIZE = BIG_CHUNK_SIZE * SMALL_CHUNK_SIZE;

class Block {
public:
	Block();

public:
	//char chunk_index; // 통합한 1차원 인덱스에 블록 타입으로 블록의 유무를 판별
	//int ix, iy, iz;
	//int index;
	char blockType;
};

class Map
{
public:
	Map();
	~Map();
	//void CreateMapData();
	bool AddBlockToMap(CS_ADD_BLOCK_PACKET* packet);
	bool RemoveBlockToMap(CS_REMOVE_BLOCK_PACKET* packet);
	void ReadMapData();
	int calculateIndex(int x, int y, int z);

public:
	std::vector<std::vector<Block>> block_map;
};

