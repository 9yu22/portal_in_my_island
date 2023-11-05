#pragma once
#define WOOD 1
#define ROCK 2
#define IRON 3

#define NONE 11
#define ADD_BLOCK 12
#define REMOVE_BLOCK 13

#define BUFFERSIZE 1024
#define SERVERPORT 8000

struct packetHeader {
	char packetSize; 
	char packetType;
};

struct blockInfo {
	char packetSize;
	char packetType;
	float x, y, z;
	char blockType;
};

void InitBlockList();
void SendBlockList(SOCKET client_sock);
void RecvPacket(SOCKET client_sock);
void AddGameBlock(const blockInfo block);
void RemoveGameBlock(const blockInfo block);
void SendGameBlocks(SOCKET client_sock);