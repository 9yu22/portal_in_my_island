// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcessQueue.h"

ProcessQueueBlock::ProcessQueueBlock()
{
}

ProcessQueueBlock::~ProcessQueueBlock()
{
}

bool ProcessQueueBlock::CheckEmpty()
{
	queue_mutex.lock();
	bool ret = BlockQ.IsEmpty();
	queue_mutex.unlock();
	return ret;
}

void ProcessQueueBlock::EnQ(const BlockInfo& block)
{
	queue_mutex.lock();
	BlockQ.Enqueue(block);
	queue_mutex.unlock();
}

BlockInfo ProcessQueueBlock::DeQ()
{
	queue_mutex.lock();
	BlockInfo block;
	BlockQ.Dequeue(block);
	queue_mutex.unlock();
	return block;
}

