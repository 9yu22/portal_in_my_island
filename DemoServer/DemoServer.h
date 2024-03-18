//# include "Common.h"

#define WOOD 1
#define ROCK 2
#define IRON 3

#define NONE 11
#define ADD_BLOCK 12
#define REMOVE_BLOCK 13
#define LOCATION 90
#define TEST 100

#define BUFFERSIZE 1024
#define SERVERPORT 4040

struct packetHeader {
	char packetSize;
	char packetType;
};

struct BlockPacket {
	char packetSize;
	char packetType;
	float x, y, z;
	char blockType;
};

struct LocationPacket {
	char packetSize;
	char packetType;
	float x;
	float y;
	float z;
};

struct TestPacket {
	char packetSize;
	char packetType;
	short number;
};

void InitBlockList();
void SendBlockList(SOCKET client_sock);
void RecvPacket(SOCKET client_sock);
void AddGameBlock(const BlockPacket block);
void RemoveGameBlock(const BlockPacket block);
void SendGameBlocks(SOCKET client_sock);