#pragma once

#include <mutex>
#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "../Private/Voxel/BKEnum.h"
//#include "../Private/BKNaiveChunk.h"

struct BlockInfo {
	FIntVector index;
	BKEBlock type;
};

class BLOCKERS_API ProcessQueueBlock
{
	TQueue<BlockInfo> BlockQ;
	std::mutex queue_mutex; // 비어있는 큐가 있다고 가정할 때 enqueue가 완전히 되지 않아도 empty가 비었다고 반환하는지에 대한 정보가 없어서 전체 뮤텍스 사용(dequeue는 empty가 아니면 수행)
public:
	ProcessQueueBlock();
	~ProcessQueueBlock();

	bool CheckEmpty();
	void EnQ(const BlockInfo& block);
	BlockInfo DeQ();
};
