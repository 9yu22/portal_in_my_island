#pragma once

#include <mutex>
#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "../Private/Voxel/BKEnum.h"
#include "Protocol.h"
//#include "../Private/BKNaiveChunk.h"

//struct BlockInfo {
//	FIntVector index;
//	BKEBlock type;
//};
//
//class BLOCKERS_API ProcessQueueBlock
//{
//	TQueue<BlockInfo> BlockQ;
//	std::mutex queue_mutex; // 비어있는 큐가 있다고 가정할 때 enqueue가 완전히 되지 않아도 empty가 비었다고 반환하는지에 대한 정보가 없어서 전체 뮤텍스 사용(dequeue는 empty가 아니면 수행)
//public:
//	ProcessQueueBlock();
//	~ProcessQueueBlock();
//
//	bool CheckEmpty();
//	void EnQ(BlockInfo& block);
//	BlockInfo DeQ();
//};

struct BlockInfo {
    int8 chunk_index;
    FIntVector index;
    BKEBlock type;
};

struct AnimInfo {
    int8 id;
    Anim type;
};

template <typename T>
class ProcessQueue
{
private:
    TQueue<T> BlockQ;
    std::mutex queue_mutex; // 비어있는 큐가 있다고 가정할 때 enqueue가 완전히 되지 않아도 empty가 비었다고 반환하는지에 대한 정보가 없어서 전체 뮤텍스 사용(dequeue는 empty가 아니면 수행)

public:
    ProcessQueue();
    ~ProcessQueue();

    bool CheckEmpty();
    void EnQ(const T& block);
    T DeQ();
};

template <typename T>
ProcessQueue<T>::ProcessQueue()
{
}

template <typename T>
ProcessQueue<T>::~ProcessQueue()
{
}

template <typename T>
bool ProcessQueue<T>::CheckEmpty()
{
    queue_mutex.lock();
    bool ret = BlockQ.IsEmpty();
    queue_mutex.unlock();
    return ret;
}

template <typename T>
void ProcessQueue<T>::EnQ(const T& block)
{
    queue_mutex.lock();
    BlockQ.Enqueue(block);
    queue_mutex.unlock();
}

template <typename T>
T ProcessQueue<T>::DeQ()
{
    queue_mutex.lock();
    T block;
    BlockQ.Dequeue(block);
    queue_mutex.unlock();
    return block;
}