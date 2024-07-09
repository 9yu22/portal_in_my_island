#include "Map.h"
#include "protocol.h"

Map::Map()
{
    block_map.resize(BIG_CHUNK_SIZE, std::vector<Block>(SMALL_CHUNK_SIZE));
    ReadMapData();
}

Map::~Map()
{
}

bool Map::AddBlockToMap(CS_ADD_BLOCK_PACKET* packet)
{
    if (packet->blocktype != static_cast<int>(BKEBlock::Air) && packet->chunk_index >= 0) {
        int index = calculateIndex(packet->ix, packet->iy, packet->iz);
        if (block_map[packet->chunk_index][index].blockType == static_cast<int>(BKEBlock::Air)) {
            block_map[packet->chunk_index][index].blockType = packet->blocktype;
            return true;
        }
        return false;
    }
    return false;
}

bool Map::RemoveBlockToMap(CS_REMOVE_BLOCK_PACKET* packet)
{
    if (packet->blocktype == static_cast<int>(BKEBlock::Air) && packet->chunk_index >= 0) {
        int index = calculateIndex(packet->ix, packet->iy, packet->iz);
        if (block_map[index][packet->chunk_index].blockType != static_cast<int>(BKEBlock::Air)) {
            block_map[index][packet->chunk_index].blockType = packet->blocktype;
            //std::cout << "x: " << packet->ix << " y: " << packet->iy << " z: " << packet->iz << "위치에 블록 추가" << std::endl;
            return true;
        }
        return false;
    }
    return false;
}

void Map::ReadMapData()
{
    std::ifstream file("Blockers_map.txt");
    if (!file) {
        std::cerr << "파일을 열 수 없습니다.\n";
        return;
    }

    int chunk_index; // char로 숫자를 읽어오면 아스키 코드가 저장됨
    int x, y, z;
    int blockType;
    int blockCount = 0;
    int crossIndex = 0;

    while (file >> chunk_index >> x >> y >> z >> blockType) {
        int index = calculateIndex(x, y, z);

        Block temp;
        temp.blockType = blockType;

        if (block_map[chunk_index][index].blockType == static_cast<int>(BKEBlock::Air)) {
            block_map[chunk_index][index] = temp;
            blockCount++;
        }
        else
            crossIndex++;
    }
    std::cout << "생성된 맵 블록 수: " << blockCount << std::endl;
    //std::cout << "중복 생성된 맵 블록 수: " << crossIndex << std::endl;
    file.close();
}

int Map::calculateIndex(int x, int y, int z)
{
    int index = (x + (y * SMALL_CHUNK_WIDTH) + (z * SMALL_CHUNK_WIDTH * SMALL_CHUNK_WIDTH));

    return index;
}

Block::Block()
{
    blockType = static_cast<int>(BKEBlock::Air);
}
